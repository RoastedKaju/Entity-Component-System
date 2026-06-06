#ifndef WORLD_HPP
#define WORLD_HPP

#include "Common.hpp"
#include "Entity.hpp"

// Texture wrapper
struct Texture
{
    SDL_Texture *texture;
    const std::string path;

    Texture(SDL_Renderer *renderer, const std::string &path) : path{path}
    {
        texture = IMG_LoadTexture(renderer, std::string(ASSET_DIR + path).c_str());
        std::cout << "Texture loaded: " << path << std::endl;
    }

    ~Texture()
    {
        std::cout << "Texture destroyed: " << path << std::endl;
    }
};

// Component
struct BackgroundComponent
{
    Texture *texture;
};

// System
struct ISystem
{
    virtual ~ISystem() = default;
    virtual void Update(Scene &scene, float deltaTime) = 0;
};

struct IRenderSystem
{
    virtual ~IRenderSystem() = default;
    virtual void Render(Scene &scene, SDL_Renderer *renderer) = 0;
};

struct BackgroundSystem : public IRenderSystem
{
    BackgroundSystem()
    {
        std::cout << "Background System Initialized.\n";
    }

    ~BackgroundSystem() override
    {
        std::cout << "Background System Destroyed.\n";
    }

    void Render(Scene &scene, SDL_Renderer *renderer) override
    {
        auto view = SceneView<BackgroundComponent>(scene);

        for (auto entity : view)
        {
            auto *backgroundComp = scene.GetComponent<BackgroundComponent>(entity);

            SDL_FRect rect{0, 0, 1024, 1024};

            SDL_RenderTexture(renderer, backgroundComp->texture->texture, nullptr, &rect);
        }
    }
};

class World
{
public:
    World(SDL_Renderer *renderer);
    ~World();

    void Init();
    void Update(float deltaTime);
    void Render();

private:
    SDL_Renderer *renderer;

    // Entity Manager
    Scene scene;

    // Textures
    std::unique_ptr<Texture> backgroundTexture;

    // Systems
    std::vector<std::unique_ptr<ISystem>> updateSystems;
    std::vector<std::unique_ptr<IRenderSystem>> renderSystems;
};

#endif // WORLD_HPP