#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP

#include "Common.hpp"
#include "Entity.hpp"
#include "EntityConstants.hpp"

template <typename Tag, typename ComponentType>
class ComponentManager
{
public:
    ComponentManager()
    {
        m_sparse.fill(INVALID_INDEX);
    }

    void AddComponent(Entity<Tag> entity, ComponentType component)
    {
        if (HasComponent(entity))
        {
            throw std::runtime_error("Entity Already has component.");
        }

        uint64_t entityId = entity.Get();
        size_t denseIndex = m_size;

        m_sparse[entityId] = denseIndex;
        m_dense[denseIndex] = component;
        m_entityMap[denseIndex] = entityId;

        m_size++;
    }

    void RemoveComponent(Entity<Tag> entity)
    {
        if (!HasComponent(entity))
        {
            throw std::runtime_error("Entity does not have component.");
        }

        uint64_t entityId = entity.Get();
        size_t denseIndex = m_sparse[entityId];

        // Swap with last element to keep array packed
        size_t lastDense = m_size - 1;
        uint64_t lastEntityId = m_entityMap[lastDense];

        m_dense[denseIndex] = m_dense[lastDense];
        m_entityMap[denseIndex] = lastEntityId;
        m_sparse[lastEntityId] = denseIndex;

        m_sparse[entityId] = INVALID_INDEX;
        m_size--;
    }

    ComponentType &GetComponent(Entity<Tag> entity)
    {
        if (!HasComponent(entity))
        {
            throw std::runtime_error("Entity does not have component");
        }

        return m_dense[m_sparse[entity.Get()]];
    }

    bool HasComponent(Entity<Tag> entity)
    {
        uint64_t id = entity.Get();
        return id < MAX_ENTITIES && m_sparse[id] != INVALID_INDEX;
    }

    size_t Size() const { return m_size; }

private:
    std::array<ComponentType, MAX_ENTITIES> m_dense{};
    std::array<size_t, MAX_ENTITIES> m_sparse{};
    std::array<uint64_t, MAX_ENTITIES> m_entityMap{};

    size_t m_size{0};
};

struct DefaultComponent
{
    int value;
};

#endif // COMPONENT_MANAGER_HPP