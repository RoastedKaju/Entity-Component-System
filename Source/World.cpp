#include "World.hpp"

World::World(SDL_Renderer *renderer) : renderer{renderer}
{
}

World::~World()
{
}

void World::Init()
{
    // Load resources
    backgroundTexture = std::make_unique<Texture>(renderer, "Background.png");

    // Make Entites And Attach Components
    EntityId backgroundEntity = scene.CreateEntity();
    auto *backgroundComp = scene.AddComponent<BackgroundComponent>(backgroundEntity);
    backgroundComp->texture = backgroundTexture.get();

    // Make systems
    renderSystems.push_back(std::make_unique<BackgroundSystem>());
}

void World::Update(float deltaTime)
{
    (void)deltaTime;
}

void World::Render()
{
    for (auto &system : renderSystems)
    {
        system->Render(scene, renderer);
    }
}
