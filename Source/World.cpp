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
    EntityID backgroundEntity = scene.CreateEntity();
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
#if USE_ACTORS
    while (actors.size() < desiredBotCount)
    {
        AddBot();
    }

    while (actors.size() > desiredBotCount)
    {
        RemoveBot();
    }
#else
    while (bots.size() < desiredBotCount)
    {
        AddBot();
    }
    while (bots.size() > desiredBotCount)
    {
        RemoveBot();
    }
#endif

    for (auto &system : updateSystems)
    {
        system->Update(scene, deltaTime);
    }

#if USE_ACTORS
    int logicalWidth, logicalHeight;
    SDL_GetRenderLogicalPresentation(renderer, &logicalWidth, &logicalHeight, nullptr);
    for (auto &actor : actors)
    {
        actor->update(deltaTime, (float)logicalWidth, (float)logicalHeight);
    }
#endif
}

void World::Render()
{
    for (auto &system : renderSystems)
    {
        system->Render(scene, renderer);
    }

#if USE_ACTORS
    for (auto &actor : actors)
    {
        actor->render(renderer);
    }
#endif
}

void World::AddBot()
{
    // Logical screen size
    int logicalWidth, logicalHeight;
    SDL_GetRenderLogicalPresentation(renderer, &logicalWidth, &logicalHeight, nullptr);

    // Pick a random location on our logical screen
    const float x = RandomFloat(0.0f, (float)logicalWidth);
    const float y = RandomFloat(0.0f, (float)logicalHeight);

#if USE_ACTORS
    actors.push_back(std::make_unique<Actor>(x, y, botTexture.get()));
    std::cout << "Non-ECS actor Added to World.\n";
#else
    // TODO: Add helpful error if let's say texture is null
    bots.push_back(scene.CreateEntity());
    scene.AddComponent<SpriteComponent>(bots.back(), botTexture.get());
    scene.AddComponent<TransformComponent>(bots.back(), x, y);
    scene.AddComponent<WanderComponent>(bots.back());
#endif
}

void World::RemoveBot()
{
#if USE_ACTORS
    if (actors.empty())
    {
        return;
    }

    actors.pop_back();
#else
    if (bots.empty())
    {
        return;
    }

    EntityID entity = bots.back();
    bots.pop_back();

    scene.DestroyEntity(entity);
#endif
}
