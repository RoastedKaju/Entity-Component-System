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

    int width = 640;
    int height = 480;
    window = SDL_CreateWindow("ECS Application", width, height, SDL_WINDOW_RESIZABLE);
    check(window != nullptr, "Failed to create Window.");

    renderer = SDL_CreateRenderer(window, nullptr);
    check(renderer != nullptr, "Failed to create Renderer.");

    SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    // Create World
    world = std::make_unique<World>(renderer);

    world->Init();
}

void Application::Run()
{
    running = true;

    Uint64 previousTime = SDL_GetPerformanceCounter();

    SDL_Event event;
    while (running)
    {
        Uint64 currentTime = SDL_GetPerformanceCounter();

        float deltaTime = static_cast<float>(currentTime - previousTime) / static_cast<float>(SDL_GetPerformanceFrequency());
        previousTime = currentTime;

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.scancode == SDL_SCANCODE_RETURN && event.key.mod & SDL_KMOD_ALT)
                {
                    SDL_WindowFlags flags = SDL_GetWindowFlags(window);
                    if (flags & SDL_WINDOW_FULLSCREEN)
                    {
                        // Back to windowed
                        SDL_SetWindowFullscreen(window, false);
                        SDL_SetWindowBordered(window, true);
                    }
                    else
                    {
                        // Go to fullscreen mode
                        SDL_SetWindowFullscreen(window, true);
                        SDL_SetWindowBordered(window, false);
                    }
                }
            }
        }

        // Update Logic
        world->Update(deltaTime);

        // Set clear color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render world
        world->Render();

        // ImGui
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Debug");

        ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
        ImGui::Text("Delta Time: %.4f", deltaTime);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

        // Present
        SDL_RenderPresent(renderer);
    }
}

void Application::Shutdown()
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    if (world)
    {
        world.reset();
    }
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