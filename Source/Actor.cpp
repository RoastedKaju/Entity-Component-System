#include "Actor.hpp"

Actor::Actor(float x, float y, Texture *texture)
{
    transformComp = std::make_unique<TransformComponent>(x, y);
    spriteComp = std::make_unique<SpriteComponent>(texture);
    wanderComp = std::make_unique<WanderComponent>();
}

Actor::~Actor()
{
}

void Actor::update(float deltaTime, const float logicalWidth, const float logicalHeight)
{
    // Move in current direction
    transformComp->x += wanderComp->dx * wanderComp->speed * deltaTime;
    transformComp->y += wanderComp->dy * wanderComp->speed * deltaTime;

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

void Actor::render(SDL_Renderer *renderer)
{
    SDL_FRect rect{transformComp->x, transformComp->y, 32, 42};
    SDL_RenderTexture(renderer, spriteComp->texture->GetSDLTexture(), nullptr, &rect);
}
