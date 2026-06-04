#ifndef WORLD_HPP
#define WORLD_HPP

#include "Common.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "ComponentManager.hpp"

class World
{
public:
    World();
    ~World();

    template <typename ComponentType>
    void RegisterComponent(const std::string &debugName = "")
    {
        m_componentManagers.push_back(std::make_unique<ComponentManager<DefaultEntity, ComponentType>>(debugName));
        std::cout << "Registered Component Type: " << m_componentManagers.back()->GetDebugName() << '\n';
    }

    void DestroyEntity(Entity<DefaultEntity> entity);
    
    void PrintAllComponentManagers() const;

private:
    // Entities
    EntityManager<DefaultEntity> m_entityManager;

    // Component managers
    std::vector<std::unique_ptr<IComponentManager>> m_componentManagers;
};

#endif // WORLD_HPP