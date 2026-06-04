#include "EntityManager.hpp"
#include "EntityConstants.hpp"

ID<EntityTag> EntityManager::CreateEntity()
{
    if (EntityCount() >= MAX_ENTITIES)
    {
        throw std::runtime_error("Maximum number of entities reached");
    }

    // Check if there are free slots available for reuse
    if (!m_freeSlots.empty())
    {
        size_t index = m_freeSlots.front();
        m_freeSlots.pop();
        return ID<EntityTag>(index);
    }

    return ID<EntityTag>(m_entityCount++);
}

void EntityManager::DestroyEntity(ID<EntityTag> id)
{
    m_freeSlots.push(id.Get());
}

size_t EntityManager::EntityCount() const
{
    return m_entityCount - m_freeSlots.size();
}
