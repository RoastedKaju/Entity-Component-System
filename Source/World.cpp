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

    for (size_t i = 0; i < desiredBotCount; ++i)
    {
        AddBot();
    }

    // Make systems
    updateSystems.push_back(std::make_unique<MovementSystem>(renderer));
    renderSystems.push_back(std::make_unique<RenderingSystem>());
}

void World::Update(float deltaTime)
{
    while (bots.size() < desiredBotCount)
    {
        AddBot();
    }

    while (bots.size() > desiredBotCount)
    {
        RemoveBot();
    }

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

void World::AddBot()
{
    // Logical screen size
    int logicalWidth, logicalHeight;
    SDL_GetRenderLogicalPresentation(renderer, &logicalWidth, &logicalHeight, nullptr);

    // Pick a random location on our logical screen
    const float x = RandomFloat(0.0f, (float)logicalWidth);
    const float y = RandomFloat(0.0f, (float)logicalHeight);

    // TODO: Add helpful error if let's say texture is null
    bots.push_back(scene.CreateEntity());
    scene.AddComponent<SpriteComponent>(bots.back(), botTexture.get());
    scene.AddComponent<TransformComponent>(bots.back(), x, y);
    scene.AddComponent<WanderComponent>(bots.back());
}

void World::RemoveBot()
{
    if (bots.empty())
    {
        return;
    }

    EntityId entity = bots.back();
    bots.pop_back();

    scene.DestroyEntity(entity);
}
