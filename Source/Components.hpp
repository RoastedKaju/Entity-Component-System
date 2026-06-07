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

#endif // COMPONENTS_HPP