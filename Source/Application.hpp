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

    void SpawnEntities();

    std::unique_ptr<class EntityManager> entityManager;
};

#endif // APPLICATION_HPP