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

    int width, height;
    SDL_GetRenderOutputSize(renderer, &width, &height);

    for (auto entity : view)
    {
        auto *backgroundComp = scene.GetComponent<BackgroundComponent>(entity);

        // SDL_FRect rect{0, 0, (float)width, (float)height};
        SDL_FRect rect{0, 0, 640, 480};

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

MovementSystem::MovementSystem()
{
    std::cout << "Movement System Initialized.\n";
}

MovementSystem::~MovementSystem()
{
    std::cout << "Movement System Destroyed.\n";
}

void MovementSystem::Update(Scene &scene, float deltaTime)
{
    auto view = scene.View<TransformComponent>();

    for (auto entity : view)
    {
        auto *transformComp = scene.GetComponent<TransformComponent>(entity);

        transformComp->x += 0.01f;
    }
}
