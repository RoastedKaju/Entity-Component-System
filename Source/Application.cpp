#include "Application.hpp"

Application::Application() : window(nullptr), renderer(nullptr), running(false)
{
}

Application::~Application()
{
    Shutdown();
}

void Application::Init()
{
    check(SDL_Init(SDL_INIT_VIDEO), "Failed to initialize SDL");

    window = SDL_CreateWindow("ECS Application", 800, 600, SDL_WINDOW_RESIZABLE);
    check(window != nullptr, "Failed to create Window.");

    renderer = SDL_CreateRenderer(window, nullptr);
    check(renderer != nullptr, "Failed to create Renderer.");

    // Spawn world
    m_world = std::make_unique<World>();
    // Register components
    m_world->RegisterComponent<BaseComponent>("Default Component");

    // Create Entity
    auto entity = m_world->CreateEntity();

    // Add components to entity
    m_world->AddComponent(entity, BaseComponent{.value = 69});

    // Remove Component from entity
    m_world->RemoveComponent<BaseComponent>(entity);

    m_world->AddComponent<BaseComponent>(entity, BaseComponent{.value = 70});

    // Destroy Entity
    m_world->DestroyEntity(entity);
}

void Application::Run()
{
    running = true;

    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        // Set clear color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Present
        SDL_RenderPresent(renderer);
    }
}

void Application::Shutdown()
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();

    std::cout << "Successfully shutdown application." << std::endl;
}

// void Application::SpawnEntities()
// {
//     m_entityManager = std::make_unique<EntityManager<DefaultEntity>>();
//     m_componentManager = std::make_unique<ComponentManager<DefaultEntity, DefaultComponent>>();

//     std::vector<Entity<DefaultEntity>> entities;
//     entities.reserve(5);

//     // Spawn some entities
//     for (auto i = 0; i < 5; ++i)
//     {
//         auto entity = m_entityManager->CreateEntity();
//         entities.push_back(entity);

//         // Add component to entity
//         m_componentManager->AddComponent(entity, DefaultComponent{.value = i + 1});
//     }

//     // Print component data at entity 0
//     std::cout << "Component data for Entity 0 is: " << m_componentManager->GetComponent(entities.at(0)).value << std::endl;

//     // Print entity IDs
//     for (const auto &entity : entities)
//     {
//         std::cout << entity.Get() << '|';
//     }
//     std::cout << '\n';

//     // Print total entity count
//     std::cout << "Total entity count is: " << m_entityManager->EntityCount() << '\n';

//     // Destroy one and print count again
//     m_entityManager->DestroyEntity(entities.at(0));
//     m_componentManager->RemoveComponent(entities.at(0));
//     m_entityManager->DestroyEntity(entities.at(4));
//     m_componentManager->RemoveComponent(entities.at(4));
//     std::cout << "New Total entity count is: " << m_entityManager->EntityCount() << '\n';

//     // Create a new entity which will take the slot of first entity that was destroyed
//     std::cout << "New Entity At Slot: " << m_entityManager->CreateEntity().Get() << std::endl;
// }
