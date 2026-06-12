#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "Common.hpp"
#include "Texture.hpp"
#include "Components.hpp"

class Actor
{
public:
    Actor(float x, float y, Texture *texture);
    ~Actor();

    void update(float deltaTime, const float logicalWidth, const float logicalHeight);

    void render(SDL_Renderer *renderer);

private:
    std::unique_ptr<TransformComponent> transformComp;
    std::unique_ptr<SpriteComponent> spriteComp;
    std::unique_ptr<WanderComponent> wanderComp;
};

#endif //! ACTOR_HPP