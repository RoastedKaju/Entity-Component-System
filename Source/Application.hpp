#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Common.hpp"
#include "World.hpp"

class Application
{
public:
    Application();
    ~Application();

    void Init();
    void Run();
    void Shutdown();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;

    std::unique_ptr<World> m_world;
};

#endif // APPLICATION_HPP