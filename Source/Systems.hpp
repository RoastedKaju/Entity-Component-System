#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "Common.hpp"
#include "Entity.hpp"
#include "Components.hpp"

// System
struct ISystem
{
    virtual ~ISystem() = default;
    virtual void Update(Scene &scene, float deltaTime) = 0;
};

// Render System Interface
struct IRenderSystem
{
    virtual ~IRenderSystem() = default;
    virtual void Render(Scene &scene, SDL_Renderer *renderer) = 0;
};

struct RenderingSystem : public IRenderSystem
{
    RenderingSystem();
    ~RenderingSystem() override;

    void Render(Scene &scene, SDL_Renderer *renderer) override;
};

struct MovementSystem : public ISystem
{
    MovementSystem();
    ~MovementSystem() override;

    void Update(Scene &scene, float deltaTime) override;
};

#endif // SYSTEMS_HPP