#include "Texture.hpp"

Texture::Texture(SDL_Renderer *renderer, const std::string &path) : path{path}
{
    const std::string fullPath = std::string(ASSET_DIR) + path;
    texture = IMG_LoadTexture(renderer, fullPath.c_str());
    if (!texture)
    {
        std::cout << "Failed to load Texture: " << SDL_GetError() << '\n';
    }
    else
    {
        std::cout << "Texture loaded: " << path << std::endl;
    }
}

Texture::~Texture()
{
    std::cout << "Texture destroyed: " << path << std::endl;
}
