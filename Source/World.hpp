#ifndef WORLD_HPP
#define WORLD_HPP

#include "Common.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Component.hpp"
#include "ComponentManager.hpp"

class World
{
public:
    World();
    ~World();

    template <typename ComponentType>
    void RegisterComponent(const std::string &debugName = "")
    {
        uint64_t componentTypeId = ComponentTypeID::Get<ComponentType>();
        m_componentManagers[componentTypeId] = std::make_unique<ComponentManager<BaseEntity, ComponentType>>(debugName);
        std::cout << "Registered Component Type: " << m_componentManagers[componentTypeId]->GetDebugName() << '\n';
    }

    Entity<BaseEntity> CreateEntity();
    void DestroyEntity(Entity<BaseEntity> entity);

    template <typename ComponentType>
    void AddComponent(Entity<BaseEntity> entity, const ComponentType &component)
    {
        uint64_t componentTypeId = ComponentTypeID::Get<ComponentType>();
        auto it = m_componentManagers.find(componentTypeId);
        if (it != m_componentManagers.end())
        {
            ComponentManager<BaseEntity, ComponentType> *manager = static_cast<ComponentManager<BaseEntity, ComponentType> *>(it->second.get());
            manager->AddComponent(entity, component);
            std::cout << "Component Added of Type: " << it->second.get()->GetDebugName() << " To Entity: " << entity.Get() << std::endl;
        }
    }

    template <typename ComponentType>
    void RemoveComponent(Entity<BaseEntity> entity)
    {
        uint64_t componentTypeId = ComponentTypeID::Get<ComponentType>();
        auto it = m_componentManagers.find(componentTypeId);
        if (it != m_componentManagers.end())
        {
            auto *manager = static_cast<ComponentManager<BaseEntity, ComponentType> *>(it->second.get());
            manager->RemoveComponent(entity);
            std::cout << "Component Removed of Type: " << it->second.get()->GetDebugName() << " From Entity: " << entity.Get() << std::endl;
        }
    }

    void PrintAllComponentManagers() const;

private:
    // Entities
    EntityManager<BaseEntity> m_entityManager;

    // Component managers
    std::unordered_map<uint64_t, std::unique_ptr<IComponentManager>> m_componentManagers;
};

#endif // WORLD_HPP