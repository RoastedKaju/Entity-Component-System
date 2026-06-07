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

    int width, height;
    SDL_GetRenderOutputSize(renderer, &width, &height);

    for (auto entity : view)
    {
        auto *backgroundComp = scene.GetComponent<BackgroundComponent>(entity);

        // SDL_FRect rect{0, 0, (float)width, (float)height};
        SDL_FRect rect{0, 0, 640, 480};

        SDL_RenderTexture(renderer, backgroundComp->texture->GetSDLTexture(), nullptr, &rect);
    }
}
