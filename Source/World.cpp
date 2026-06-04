#include "World.hpp"

World::World()
{
    std::cout << "Creating World.\n";
}

World::~World()
{
    std::cout << "Tearing down World.\n";
}

Entity<BaseEntity> World::CreateEntity()
{
    return m_entityManager.CreateEntity();
}

void World::DestroyEntity(Entity<BaseEntity> entity)
{
    m_entityManager.DestroyEntity(entity);

    // Call on entity destroyed on components
    for (auto &[componentTypeID, componentManager] : m_componentManagers)
    {
        componentManager->OnEntityDestroyed(entity.Get());
    }
}

void World::PrintAllComponentManagers() const
{
    for (const auto &[componentTypeID, componentManager] : m_componentManagers)
    {
        std::cout << componentManager->GetDebugName() << '\n';
    }
}
