#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Common.hpp"

class Texture
{
public:
    Texture(SDL_Renderer *renderer, const std::string &path);
    ~Texture();

    inline SDL_Texture *GetSDLTexture() const { return texture; }
    inline const std::string &GetPath() const { return path; }

private:
    SDL_Texture *texture;
    std::string path;
};

#endif // TEXTURE_HPP