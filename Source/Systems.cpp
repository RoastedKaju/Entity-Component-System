#include "Systems.hpp"

RenderingSystem::RenderingSystem()
{
    std::cout << "Rendering System Initialized.\n";
}

RenderingSystem::~RenderingSystem()
{
    std::cout << "Background System Destroyed.\n";
}

void RenderingSystem::Render(Scene &scene, SDL_Renderer *renderer)
{
    // Render background
    auto view = scene.View<BackgroundComponent>();

    int logicalWidth, logicalHeight;
    SDL_GetRenderLogicalPresentation(renderer, &logicalWidth, &logicalHeight, nullptr);

    for (auto entity : view)
    {
        auto *backgroundComp = scene.GetComponent<BackgroundComponent>(entity);

        SDL_FRect rect{0, 0, (float)logicalWidth, (float)logicalHeight};

        SDL_RenderTexture(renderer, backgroundComp->texture->GetSDLTexture(), nullptr, &rect);
    }

    // Render sprites
    auto spriteView = scene.View<SpriteComponent, TransformComponent>();

    for (auto entity : spriteView)
    {
        auto *spriteComp = scene.GetComponent<SpriteComponent>(entity);
        auto *transformComp = scene.GetComponent<TransformComponent>(entity);

        SDL_FRect rect{transformComp->x, transformComp->y, 32, 42};

        SDL_RenderTexture(renderer, spriteComp->texture->GetSDLTexture(), nullptr, &rect);
    }
}

MovementSystem::MovementSystem(SDL_Renderer *renderer) : renderer{renderer}
{
    std::cout << "Movement System Initialized.\n";
}

MovementSystem::~MovementSystem()
{
    std::cout << "Movement System Destroyed.\n";
}

void MovementSystem::Update(Scene &scene, float deltaTime)
{
    auto view = scene.View<TransformComponent, WanderComponent>();
    const float speed = 25.0f;

    int logicalWidth, logicalHeight;
    SDL_GetRenderLogicalPresentation(renderer, &logicalWidth, &logicalHeight, nullptr);

    for (auto entity : view)
    {
        auto *transformComp = scene.GetComponent<TransformComponent>(entity);
        auto *wanderComp = scene.GetComponent<WanderComponent>(entity);

        // Move in current direction
        transformComp->x += wanderComp->dx * speed * deltaTime;
        transformComp->y += wanderComp->dy * speed * deltaTime;

        // Check bounds
        bool hitEdge = false;
        if (transformComp->x < 0)
        {
            transformComp->x = 0;
            hitEdge = true;
        }
        if (transformComp->x > static_cast<float>(logicalWidth - 32))
        {
            transformComp->x = static_cast<float>(logicalWidth - 32);
            hitEdge = true;
        }
        if (transformComp->y < 0)
        {
            transformComp->y = 0;
            hitEdge = true;
        }
        if (transformComp->y > static_cast<float>(logicalHeight - 42))
        {
            transformComp->y = static_cast<float>(logicalHeight - 42);
            hitEdge = true;
        }

        // If we hit an edge, pick a new random direction
        if (hitEdge)
        {
            float angle = RandomFloat(0.0f, 2.0f * 3.1415926535f);

            wanderComp->dx = std::cos(angle);
            wanderComp->dy = std::sin(angle);
        }
    }

    // Wandering
}
