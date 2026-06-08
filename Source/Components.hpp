#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "Common.hpp"
#include "Texture.hpp"

struct BackgroundComponent
{
    BackgroundComponent() = default;
    explicit BackgroundComponent(Texture *texture) : texture{texture} {}

    Texture *texture;
};

struct SpriteComponent
{
    SpriteComponent() = default;
    explicit SpriteComponent(Texture *texture) : texture{texture} {}

    Texture *texture;
};

struct TransformComponent
{
    TransformComponent() = default;
    TransformComponent(float x, float y) : x{x}, y{y} {}

    float x{0.0f};
    float y{0.0f};
};

struct WanderComponent
{
    WanderComponent()
    {
        float angle = RandomFloat(0.0f, 2.0f * 3.1415926535f);
        dx = std::cos(angle);
        dy = std::sin(angle);
    }
    WanderComponent(float dx, float dy) : dx{dx}, dy{dy} {}

    float dx;
    float dy;
    float speed{25.0f};
};

#endif // COMPONENTS_HPP