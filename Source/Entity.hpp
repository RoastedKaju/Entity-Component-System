#ifndef Entity_COMPONENT_SYSTEM_HPP
#define Entity_COMPONENT_SYSTEM_HPP

#include "Common.hpp"

constexpr uint32_t MAX_ENTITY_COUNT = 100'000;
constexpr uint32_t MAX_COMPONENT_TYPES = 32;
constexpr uint32_t INVALID_ENTITY_INDEX = std::numeric_limits<uint32_t>::max();

using EntityID = uint64_t;
using ComponentTypeID = uint32_t;
using EntityIndex = uint32_t;
using EntityGeneration = uint32_t;
using ComponentMask = std::bitset<MAX_COMPONENT_TYPES>;

inline std::atomic<ComponentTypeID> nextComponentID = 0;
// Get component type ID
template <typename T>
ComponentTypeID GetComponentTypeID()
{
    static ComponentTypeID ID = nextComponentID.fetch_add(1, std::memory_order_relaxed);
    assert(ID < MAX_COMPONENT_TYPES && "Exceeded max component types");
    return ID;
}

// Create entity ID by combinding index and version
// First 32 bits are index, last 32 bits are version
inline EntityID CreateEntityID(EntityIndex index, EntityGeneration generation)
{
    return ((EntityID)index << 32) | ((EntityID)generation);
}
inline EntityIndex GetEntityIndex(EntityID entity)
{
    return entity >> 32;
}
inline EntityGeneration GetEntityGeneration(EntityID entity)
{
    return static_cast<EntityGeneration>(entity);
}

// struct IStorage
// {
//     virtual ~IStorage() = default;
//     virtual void *Get(size_t index) = 0;
//     virtual void Destroy(size_t index) = 0;
// };

// template <typename T>
// struct PoolStorage : public IStorage
// {
//     void *data{nullptr};
//     size_t elementSize{0};
//     size_t alignment{0};

//     PoolStorage()
//     {
//         elementSize = sizeof(T);
//         alignment = alignof(T);

//         data = ::operator new(elementSize * MAX_ENTITY_COUNT, std::align_val_t(alignment));
//     }

//     ~PoolStorage()
//     {
//         ::operator delete(data, std::align_val_t(alignment));
//     }

//     void *Get(size_t index) override
//     {
//         return static_cast<char *>(data) + index * elementSize;
//     }

//     void Destroy(size_t index) override
//     {
//         // gets the pointer to this component's memory location
//         T *component = static_cast<T *>(Get(index));
//         component->~T();
//     }
// };

struct IStorage
{
    virtual ~IStorage() = default;
    virtual bool Remove(EntityID entity) = 0;
    virtual bool Contains(EntityID entity) const = 0;
    virtual size_t Size() const = 0;
    virtual bool Empty() const = 0;
};

template <typename T>
struct SparseSet : public IStorage
{
    std::vector<EntityIndex> sparse;
    std::vector<T> dense;
    std::vector<EntityID> denseEntities;

    void EnsureSparse(EntityIndex index)
    {
        if (index >= sparse.size())
        {
            sparse.resize(index + 1, INVALID_ENTITY_INDEX);
        }
    }

    template <typename... Args>
    T *Emplace(EntityID id, Args &&...args)
    {
        EntityIndex index = GetEntityIndex(id);
        EnsureSparse(index);

        if (sparse[index] != INVALID_ENTITY_INDEX)
        {
            // overwrite existing
            dense[sparse[index]] = T(std::forward<Args>(args)...);
            return &dense[sparse[index]];
        }

        sparse[index] = static_cast<EntityIndex>(dense.size());
        dense.emplace_back(std::forward<Args>(args)...);
        denseEntities.push_back(id);

        return &dense.back();
    }

    bool Remove(EntityID id) override
    {
        EntityIndex index = GetEntityIndex(id);
        if (index >= sparse.size() || sparse[index] == INVALID_ENTITY_INDEX)
        {
            return false;
        }

        EntityIndex densePos = sparse[index];
        EntityIndex lastPos = static_cast<EntityIndex>(dense.size() - 1);

        if (densePos != lastPos)
        {
            dense[densePos] = std::move(dense[lastPos]);
            denseEntities[densePos] = denseEntities[lastPos];
            sparse[GetEntityIndex(denseEntities[densePos])] = densePos;
        }

        dense.pop_back();
        denseEntities.pop_back();
        sparse[index] = INVALID_ENTITY_INDEX;
        return true;
    }

    T *Get(EntityID id)
    {
        EntityIndex index = GetEntityIndex(id);
        if (index >= sparse.size() || sparse[index] == INVALID_ENTITY_INDEX)
        {
            return nullptr;
        }

        return &dense[sparse[index]];
    }

    const T *Get(EntityID id) const
    {
        EntityIndex index = GetEntityIndex(id);
        if (index >= sparse.size() || sparse[index] == INVALID_ENTITY_INDEX)
        {
            return nullptr;
        }
        return &dense[sparse[index]];
    }

    bool Contains(EntityID id) const override
    {
        EntityIndex index = GetEntityIndex(id);
        return index < sparse.size() && sparse[index] != INVALID_ENTITY_INDEX;
    }

    size_t Size() const override { return dense.size(); }
    bool Empty() const override { return dense.empty(); }
};

struct Scene
{
    // All the information about entity
    // minus the entity index as the entity index will map 1-1 directly with our entities vector
    struct EntityDesc
    {
        EntityGeneration generation{0};
        ComponentMask mask{};
        bool alive{false};
    };

    // tracking lists
    std::vector<EntityDesc> entities;
    std::vector<EntityIndex> freeIndices; // use stack its better for LIFO
    std::vector<std::unique_ptr<IStorage>> componentSets;
    // can also add list of alive indices, but you will have to do extra book keeping in create and destroy

    bool IsAlive(EntityID id) const
    {
        EntityIndex index = GetEntityIndex(id);

        if (index >= entities.size())
        {
            return false;
        }

        const auto &entityDesc = entities[index];

        return entityDesc.alive && entityDesc.generation == GetEntityGeneration(id);
    }

    EntityID CreateEntity()
    {
        EntityIndex index;

        // check if we can reuse some slot
        if (!freeIndices.empty())
        {
            index = freeIndices.back();
            freeIndices.pop_back();
        }
        else
        {
            assert(entities.size() < MAX_ENTITY_COUNT && "Cannot create more entities than max entity count.");

            index = static_cast<EntityIndex>(entities.size());
            entities.emplace_back(EntityDesc{});
        }

        auto &entityDesc = entities[index];
        entityDesc.alive = true;

        return CreateEntityID(index, entityDesc.generation);
    }

    void DestroyEntity(EntityID id)
    {
        if (!IsAlive(id))
        {
            return;
        }

        EntityIndex index = GetEntityIndex(id);
        auto &entityDesc = entities[index];

        // clean up all the components attached to this entity
        for (size_t i = 0; i < componentSets.size(); ++i)
        {
            if (entityDesc.mask.test(i))
            {
                componentSets[i]->Remove(id);
            }
        }

        // reset component mask
        entityDesc.mask.reset();

        // bump up the generation counter
        entityDesc.alive = false;
        ++entityDesc.generation;

        // add to free list
        freeIndices.push_back(index);
    }

    // Has component
    template <typename T>
    bool HasComponent(EntityID id)
    {
        // check if entity is alive
        if (!IsAlive(id))
        {
            return false;
        }

        const EntityIndex index = GetEntityIndex(id);
        const ComponentTypeID componentID = GetComponentTypeID<T>();

        return entities[index].mask.test(componentID);
    }

    // Add component to entity
    template <typename T, typename... Args>
    T *AddComponent(EntityID id, Args &&...args)
    {
        // check if entity is alive
        assert(IsAlive(id) && "To add a component entity should be alive");

        EntityIndex index = GetEntityIndex(id);
        ComponentTypeID componentID = GetComponentTypeID<T>();

        // check if entity already has this component
        assert(!entities[index].mask.test(componentID) && "Entity already has this component");

        // If this type already doesn't have a pool then reserve
        if (componentSets.size() <= componentID)
        {
            // since component type ids start from 0, add 1 to get the new size
            componentSets.resize(componentID + 1);
        }

        // if set is not initialized then create it
        if (!componentSets[componentID])
        {
            componentSets[componentID] = std::make_unique<SparseSet<T>>();
        }

        auto *set = static_cast<SparseSet<T> *>(componentSets[componentID].get());
        T *component = set->Emplace(id, std::forward<Args>(args)...);

        entities[index].mask.set(componentID);
        return component;
    }

    // Remove component from entity
    template <typename T>
    void RemoveComponent(EntityID id)
    {
        // check if entity is alive
        if (!IsAlive(id))
        {
            return;
        }

        EntityIndex index = GetEntityIndex(id);
        ComponentTypeID componentID = GetComponentTypeID<T>();

        // check if entity has this component
        if (!entities[index].mask.test(componentID))
        {
            return;
        }

        // destroy component
        componentSets[componentID]->Remove(id);

        // reset bit mask
        entities[index].mask.reset(componentID);
    }

    // Get Component
    template <typename T>
    T *GetComponent(EntityID id)
    {
        // Check if Entity is alive
        if (!IsAlive(id))
        {
            return nullptr;
        }

        EntityIndex index = GetEntityIndex(id);
        ComponentTypeID componentID = GetComponentTypeID<T>();

        // Check if entity has component
        if (componentID >= componentSets.size() || !componentSets[componentID])
        {
            return nullptr;
        }
        if (!entities[index].mask.test(componentID))
        {
            return nullptr;
        }

        return static_cast<SparseSet<T> *>(componentSets[componentID].get())->Get(id);
    }

    // Get Component Internal
    template <typename T>
    T *GetComponentInternal(EntityID id)
    {
        ComponentTypeID componentID = GetComponentTypeID<T>();
        if (componentID >= componentSets.size() || !componentSets[componentID])
        {
            return nullptr;
        }

        return static_cast<SparseSet<T> *>(componentSets[componentID].get())->Get(id);
    }
};

template <typename... Types>
struct View
{
    Scene &scene;
    ComponentMask requiredMask;

    View(Scene &scene) : scene{scene}
    {
        (requiredMask.set(GetComponentTypeID<Types>()), ...);
    }

    template <typename Func>
    void Each(Func &&func)
    {
        // Resolve the pool for the first (ideally rarest) component type
        using FirstType = std::tuple_element_t<0, std::tuple<Types...>>;
        ComponentTypeID firstID = GetComponentTypeID<FirstType>();

        if (firstID >= scene.componentSets.size() || !scene.componentSets[firstID])
        {
            return;
        }

        auto *firstPool = static_cast<SparseSet<FirstType> *>(scene.componentSets[firstID].get());

        for (EntityID id : firstPool->denseEntities)
        {
            EntityIndex index = GetEntityIndex(id);

            // Stale ID guard: generation must still match
            if (!scene.IsAlive(id))
            {
                continue;
            }

            // Check all other required components via bitmask
            if ((scene.entities[index].mask & requiredMask) != requiredMask)
            {
                continue;
            }

            func(id, *scene.GetComponentInternal<Types>(id)...);
        }
    }
};

#endif // Entity_COMPONENT_SYSTEM_HPP