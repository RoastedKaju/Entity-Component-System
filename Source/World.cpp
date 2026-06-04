#include "World.hpp"

World::World()
{
    std::cout << "Creating World.\n";

    for (size_t i = 0; i < 5; ++i)
    {
        auto entity = m_entityManager.CreateEntity();
        std::cout << entity.Get() << " ";
    }
    std::cout << '\n';
}

World::~World()
{
    std::cout << "Tearing down World.\n";
}

void World::DestroyEntity(Entity<DefaultEntity> entity)
{
    m_entityManager.DestroyEntity(entity);

    // Call on entity destroyed on components
    for (auto &componentManager : m_componentManagers)
    {
        componentManager->OnEntityDestroyed(entity.Get());
    }
}

void World::PrintAllComponentManagers() const
{
    for (const auto &componentManager : m_componentManagers)
    {
        std::cout << componentManager->GetDebugName() << '\n';
    }
}
