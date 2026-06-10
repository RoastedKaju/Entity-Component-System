#include "Systems.hpp"

RenderingSystem::RenderingSystem()
{
    std::cout << "Rendering System Initialized.\n";
}

RenderingSystem::~RenderingSystem()
{
    std::cout << "Rendering System Destroyed.\n";
}

void RenderingSystem::Render(Scene &scene, SDL_Renderer *renderer)
{
    int logicalWidth, logicalHeight;
    SDL_GetRenderLogicalPresentation(renderer, &logicalWidth, &logicalHeight, nullptr);

    // Render background
    View<BackgroundComponent> backgroundView{scene};
    backgroundView.Each([&](EntityID id, BackgroundComponent &backgroundComp)
                        {
                            (void)id;
                            SDL_FRect rect{0, 0, (float)logicalWidth, (float)logicalHeight};
                            SDL_RenderTexture(renderer, backgroundComp.texture->GetSDLTexture(), nullptr, &rect); });

    // Render sprites
    View<SpriteComponent, TransformComponent> entityView{scene};
    entityView.Each([&](EntityID id, SpriteComponent &spriteComp, TransformComponent &transformComp)
                    {
                        (void)id;
                        SDL_FRect rect{transformComp.x, transformComp.y, 32, 42};
                        SDL_RenderTexture(renderer, spriteComp.texture->GetSDLTexture(), nullptr, &rect); });
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
    int logicalWidth, logicalHeight;
    SDL_GetRenderLogicalPresentation(renderer, &logicalWidth, &logicalHeight, nullptr);

    View<TransformComponent, WanderComponent> botView{scene};

    botView.Each([&](EntityID id, TransformComponent &transformComp, WanderComponent &wanderComp)
                    {
                        // Move in current direction
                        transformComp.x += wanderComp.dx * wanderComp.speed * deltaTime;
                        transformComp.y += wanderComp.dy * wanderComp.speed * deltaTime;

                        // Check bounds
                        bool hitEdge = false;
                        if (transformComp.x < 0)
                        {
                            transformComp.x = 0;
                            hitEdge = true;
                        }
                        if (transformComp.x > static_cast<float>(logicalWidth - 32))
                        {
                            transformComp.x = static_cast<float>(logicalWidth - 32);
                            hitEdge = true;
                        }
                        if (transformComp.y < 0)
                        {
                            transformComp.y = 0;
                            hitEdge = true;
                        }
                        if (transformComp.y > static_cast<float>(logicalHeight - 42))
                        {
                            transformComp.y = static_cast<float>(logicalHeight - 42);
                            hitEdge = true;
                        }

                        // If we hit an edge, pick a new random direction
                        if (hitEdge)
                        {
                            float angle = RandomFloat(0.0f, 2.0f * 3.1415926535f);

                            wanderComp.dx = std::cos(angle);
                            wanderComp.dy = std::sin(angle);
                        } });
}