#include "Texture.hpp"

Texture::Texture(SDL_Renderer *renderer, const std::string &path) : path{path}
{
    texture = IMG_LoadTexture(renderer, std::string(ASSET_DIR + path).c_str());
    std::cout << "Texture loaded: " << path << std::endl;
}

Texture::~Texture()
{
    std::cout << "Texture destroyed: " << path << std::endl;
}
