#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Common.hpp"

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
};

#endif // APPLICATION_HPP