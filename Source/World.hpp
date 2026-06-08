#ifndef WORLD_HPP
#define WORLD_HPP

#include "Common.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "Systems.hpp"

class World
{
public:
    World(SDL_Renderer *renderer);
    ~World();

    void Init();
    void Update(float deltaTime);
    void Render();

    void AddBot();
    void RemoveBot();

    size_t GetBotCount() const { return bots.size(); }

    uint32_t desiredBotCount = 10;

private:
    SDL_Renderer *renderer;

    // Entity Manager
    Scene scene;

    // Bots
    std::vector<EntityId> bots;

    // Textures
    std::unique_ptr<Texture> backgroundTexture;
    std::unique_ptr<Texture> botTexture;

    // Systems
    std::vector<std::unique_ptr<ISystem>> updateSystems;
    std::vector<std::unique_ptr<IRenderSystem>> renderSystems;
};

#endif // WORLD_HPP