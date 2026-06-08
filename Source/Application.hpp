#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Common.hpp"
#include "World.hpp"
#include "UI.hpp"

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

    std::unique_ptr<World> world;
    std::unique_ptr<UI> uiManager;
};

#endif // APPLICATION_HPP