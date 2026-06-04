#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "Entity.hpp"

/// @brief The responsibility of entity manager is to create, destroy and reuse the entity slots
class EntityManager
{
public:
    ID<EntityTag> CreateEntity();
    void DestroyEntity(ID<EntityTag> id);
    size_t EntityCount() const;

private:
    size_t m_entityCount{0};

    // Queue of free entity slots for reuse
    std::queue<size_t> m_freeSlots;
};

#endif // ENTITY_MANAGER_HPP