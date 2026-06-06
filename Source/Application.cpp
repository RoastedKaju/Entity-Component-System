#include "Application.hpp"

Application::Application() : window(nullptr), renderer(nullptr), running(false)
{
}

Application::~Application()
{
    Shutdown();
}

void Application::Init()
{
    check(SDL_Init(SDL_INIT_VIDEO), "Failed to initialize SDL");

    window = SDL_CreateWindow("ECS Application", 800, 600, SDL_WINDOW_RESIZABLE);
    check(window != nullptr, "Failed to create Window.");

    renderer = SDL_CreateRenderer(window, nullptr);
    check(renderer != nullptr, "Failed to create Renderer.");
}

void Application::Run()
{
    running = true;

    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        // Set clear color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Present
        SDL_RenderPresent(renderer);
    }
}

void Application::Shutdown()
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();

    std::cout << "Successfully shutdown application." << std::endl;
}