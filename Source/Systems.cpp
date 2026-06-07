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
    auto view = scene.View<BackgroundComponent>();

    for (auto entity : view)
    {
        auto *backgroundComp = scene.GetComponent<BackgroundComponent>(entity);

        SDL_FRect rect{0, 0, 1024, 1024};

        SDL_RenderTexture(renderer, backgroundComp->texture->GetSDLTexture(), nullptr, &rect);
    }
}
