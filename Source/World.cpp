#include "World.hpp"

World::World(SDL_Renderer *renderer) : renderer{renderer}
{
}

World::~World()
{
}

void World::Init()
{
    // Logical screen size
    int logicalWidth, logicalHeight;
    SDL_GetRenderLogicalPresentation(renderer, &logicalWidth, &logicalHeight, nullptr);

    // Load resources
    backgroundTexture = std::make_unique<Texture>(renderer, "Background.png");
    botTexture = std::make_unique<Texture>(renderer, "Robot.png");

    // Make Entites And Attach Components
    EntityId backgroundEntity = scene.CreateEntity();
    scene.AddComponent<BackgroundComponent>(backgroundEntity, backgroundTexture.get());

    for (size_t i = 0; i < 10; ++i)
    {
        // Pick a random location on our logical screen
        const float x = RandomFloat(0.0f, (float)logicalWidth);
        const float y = RandomFloat(0.0f, (float)logicalHeight);

        // TODO: Add helpful error if let's say texture is null
        EntityId botEntity = scene.CreateEntity();
        scene.AddComponent<SpriteComponent>(botEntity, botTexture.get());
        scene.AddComponent<TransformComponent>(botEntity, x, y);
        scene.AddComponent<WanderComponent>(botEntity);
    }

    // Make systems
    updateSystems.push_back(std::make_unique<MovementSystem>(renderer));
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
