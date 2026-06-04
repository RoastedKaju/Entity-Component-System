#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "Entity.hpp"
#include "EntityConstants.hpp"

template <typename Tag>
class EntityManager
{
public:
    Entity<Tag> CreateEntity()
    {
        if (EntityCount() >= MAX_ENTITIES)
        {
            throw std::runtime_error("Maximum number of entities reached");
        }

        uint64_t id;
        if (!m_freeSlots.empty())
        {
            id = m_freeSlots.front();
            m_freeSlots.pop();
        }
        else
        {
            id = m_nextId++;
        }

        m_aliveEntities.insert(id);
        return Entity<Tag>(id);
    }

    void DestroyEntity(Entity<Tag> entity)
    {
        if (!IsAlive(entity))
        {
            return;
        }

        m_aliveEntities.erase(entity.Get());
        m_freeSlots.push(entity.Get());
    }

    size_t EntityCount() const
    {
        return m_aliveEntities.size();
    }

    bool IsAlive(Entity<Tag> entity) const
    {
        return m_aliveEntities.count(entity.Get()) > 0;
    }

private:
    uint64_t m_nextId{0};
    std::queue<uint64_t> m_freeSlots;
    std::unordered_set<uint64_t> m_aliveEntities;
};

#endif // ENTITY_MANAGER_HPP