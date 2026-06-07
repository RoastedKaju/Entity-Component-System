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
    botTexture = std::make_unique<Texture>(renderer, "Robot.png");

    // Make Entites And Attach Components
    EntityId backgroundEntity = scene.CreateEntity();
    scene.AddComponent<BackgroundComponent>(backgroundEntity, backgroundTexture.get());

    // TODO: Add helpful error if texture is null
    EntityId botEntity = scene.CreateEntity();
    scene.AddComponent<SpriteComponent>(botEntity, botTexture.get());
    scene.AddComponent<TransformComponent>(botEntity);

    // Make systems
    updateSystems.push_back(std::make_unique<MovementSystem>());
    renderSystems.push_back(std::make_unique<RenderingSystem>());
}

void World::Update(float deltaTime)
{
    for (auto &system : updateSystems)
    {
        system->Update(scene, deltaTime);
    }
}

void World::Render()
{
    for (auto &system : renderSystems)
    {
        system->Render(scene, renderer);
    }
}
