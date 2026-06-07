#ifndef Entity_COMPONENT_SYSTEM_HPP
#define Entity_COMPONENT_SYSTEM_HPP

#include "Common.hpp"

constexpr uint32_t MAX_COMPONENTS_TYPES = 32;
constexpr uint32_t MAX_ENTITIES = 10'000;
constexpr uint32_t INVALID_ENTITY_INDEX = std::numeric_limits<uint32_t>::max();

using ComponentTypeId = uint32_t;
using EntityIndex = uint32_t;
using EntityVersion = uint32_t;
using EntityId = uint64_t;
using ComponentMask = std::bitset<MAX_COMPONENTS_TYPES>;

// Unique compile time ID generator for component types
inline std::atomic<ComponentTypeId> sComponentCounter = 0;
template <typename T>
ComponentTypeId GetComponentTypeId()
{
    static ComponentTypeId sComponentId = sComponentCounter.fetch_add(1, std::memory_order_relaxed);
    return sComponentId;
}

// Utility functions for getting unqiue entity ID along with its version
inline EntityId CreateEntityId(EntityIndex index, EntityVersion version)
{
    // Shift index to left 32 bits and add version at right
    return ((EntityId)index << 32) | ((EntityId)version);
}
inline EntityIndex GetEntityIndex(EntityId id)
{
    return id >> 32;
}
inline EntityVersion GetEntityVersion(EntityId id)
{
    // cast to 32 bits, loosing the 32 bits on the left leaving you with only version
    return static_cast<EntityVersion>(id);
}

// Component pools will act as a buffer where we will place our components
// This is a bit wasteful for larger projects because it allocates all the memory upfront
// Consider using sparse set instead of this for better book-keeping
// To automatically destroy all the components when entity is destroyed we introduce typed component pool
struct IComponentPool
{
    virtual ~IComponentPool() = default;

    virtual void DestroyAt(size_t index) = 0;
    virtual void *Get(size_t index) = 0;
};

template <typename T>
struct ComponentPool : IComponentPool
{
    void *pData{nullptr};
    size_t elementSize{0};
    size_t alignment{0};

    ComponentPool()
    {
        elementSize = sizeof(T);
        alignment = alignof(T);

        // :: indicates that we are calling global space, not any class specified
        // operator new unlike just new does not initialize memory, only allocates it
        pData = ::operator new(elementSize * MAX_ENTITIES, std::align_val_t(alignment));
    }

    ~ComponentPool()
    {
        ::operator delete(pData, std::align_val_t(alignment));
    }

    void DestroyAt(size_t index) override
    {
        T *component = static_cast<T *>(Get(index));
        // call the component's destructor
        component->~T();
    }

    void *Get(size_t index) override
    {
        return static_cast<char *>(pData) + index * elementSize;
    }
};

// Forward declare Scene struct
struct Scene;
template <typename... ComponentTypes>
struct SceneView
{
    Scene *pScene;
    ComponentMask mask;
    bool all{false};

    SceneView(Scene &scene) : pScene{&scene}
    {
        // set all bits of mentioned component types
        (mask.set(GetComponentTypeId<ComponentTypes>()), ...);
    }

    // All the components at entity matches our mask
    inline bool Matches(EntityIndex index) const
    {
        const auto &entity = pScene->entities[index];

        if (!entity.alive)
        {
            return false;
        }

        return (entity.mask & mask) == mask;
    }

    struct Iterator
    {
        const SceneView *view;
        EntityIndex index;

        Iterator(const SceneView *view, EntityIndex index) : view(view), index(index)
        {
            // this will give the first entity that matches our mask
            SkipInvalid();
        }

        // Loop over entities
        // if the component mask matches then break
        // else increment
        inline void SkipInvalid()
        {
            while (index < view->pScene->entities.size())
            {
                if (view->Matches(index))
                {
                    break;
                }

                ++index;
            }
        }

        EntityId operator*() const
        {
            // give back the entity id we are currently at
            const auto &entity = view->pScene->entities[index];
            return CreateEntityId(index, entity.version);
        }

        bool operator==(const Iterator &other) const
        {
            // compare two iterators
            return index == other.index;
        }

        bool operator!=(const Iterator &other) const
        {
            return !(*this == other);
        }

        Iterator &operator++()
        {
            // move iterator forward
            ++index;
            SkipInvalid();
            return *this;
        }
    };

    const Iterator begin() const
    {
        return Iterator(this, 0);
    }

    const Iterator end() const
    {
        return Iterator(this, static_cast<EntityIndex>(pScene->entities.size()));
    }
};

// Scene manages all the entities and components
// Assigns and removes components to entities
struct Scene
{
    // All the information we need about each entity
    struct EntityDesc
    {
        EntityVersion version = 1;
        ComponentMask mask;
        bool alive = false;
    };
    std::vector<EntityDesc> entities;
    std::vector<EntityIndex> freeEntities;

    // Entity creation
    inline EntityId CreateEntity()
    {
        EntityIndex index;

        if (!freeEntities.empty())
        {
            index = freeEntities.back();
            freeEntities.pop_back();
        }
        else
        {
            assert(entities.size() < MAX_ENTITIES);

            index = static_cast<EntityIndex>(entities.size());
            entities.emplace_back(EntityDesc{});
        }

        auto &entity = entities[index];
        entity.alive = true;

        return CreateEntityId(index, entity.version);
    }

    inline void DestroyEntity(EntityId id)
    {
        assert(IsAlive(id));

        EntityIndex index = GetEntityIndex(id);
        auto &entity = entities[index];

        // clean up all the components attached to this entity
        for (size_t i = 0; i < componentPools.size(); ++i)
        {
            if (entity.mask.test(i))
            {
                componentPools[i]->DestroyAt(index);
            }
        }

        // rest component mask
        entity.mask.reset();

        // bump up the version counter
        entity.alive = false;
        ++entity.version;

        // add to free list
        freeEntities.push_back(index);
    }

    inline bool IsAlive(EntityId id) const
    {
        EntityIndex index = GetEntityIndex(id);

        if (index >= entities.size())
        {
            return false;
        }

        const auto &entity = entities[index];

        return entity.alive && entity.version == GetEntityVersion(id);
    }

    // Check if entity has this component type
    template <typename T>
    bool HasComponent(EntityId id)
    {
        assert(IsAlive(id));

        EntityIndex index = GetEntityIndex(id);

        ComponentTypeId componentId = GetComponentTypeId<T>();

        return entities[index].mask.test(componentId);
    }

    // Get Component
    template <typename T>
    T *GetComponent(EntityId id)
    {
        assert(IsAlive(id));

        if (!HasComponent<T>(id))
        {
            return nullptr;
        }

        EntityIndex index = GetEntityIndex(id);

        ComponentTypeId componentId = GetComponentTypeId<T>();

        return static_cast<T *>(componentPools[componentId]->Get(index));
    }

    template <typename T, typename... Args>
    T *AddComponent(EntityId id, Args &&...args)
    {
        assert(IsAlive(id));

        EntityIndex index = GetEntityIndex(id);
        ComponentTypeId componentId = GetComponentTypeId<T>();

        // reserve space for this type of component pool
        if (componentPools.size() <= componentId)
        {
            componentPools.resize(componentId + 1);
        }

        // If component pool is not initialized
        if (!componentPools[componentId])
        {
            componentPools[componentId] = std::make_unique<ComponentPool<T>>();
        }

        // only add component if entity doesn't have it already
        assert(!HasComponent<T>(id));

        void *componentMemory = componentPools[componentId]->Get(index);

        // Placement new on this index slot inside our component pool
        T *component = new (componentMemory) T(std::forward<Args>(args)...);

        // mark the bitset that we have added this component to our entity
        entities[index].mask.set(componentId);

        return component;
    }

    template <typename T>
    void RemoveComponent(EntityId id)
    {
        assert(IsAlive(id));

        if (!HasComponent<T>(id))
        {
            return;
        }

        EntityIndex index = GetEntityIndex(id);
        ComponentTypeId componentId = GetComponentTypeId<T>();

        // free component from memory
        T *component = static_cast<T *>(componentPools[componentId]->Get(index));
        component->~T();

        // reset bit field for that component
        entities[index].mask.reset(componentId);
    }

    // A way to call scene view from scene
    template <typename... ComponentTypes>
    SceneView<ComponentTypes...> View()
    {
        return SceneView<ComponentTypes...>(*this);
    }

    // Pool storage
    std::vector<std::unique_ptr<IComponentPool>> componentPools;
};

#endif // Entity_COMPONENT_SYSTEM_HPP