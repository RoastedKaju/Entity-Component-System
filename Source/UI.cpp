#include "UI.hpp"

UI::UI(SDL_Window *window, SDL_Renderer *renderer, World *world) : window{window}, renderer{renderer}, world{world}
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
}

UI::~UI()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void UI::PollEvent(SDL_Event &event)
{
	ImGui_ImplSDL3_ProcessEvent(&event);
}

void UI::StartFrame()
{
	SDL_GetRenderLogicalPresentation(renderer, &logicalWidth, &logicalHeight, nullptr);
	SDL_SetRenderLogicalPresentation(renderer, 0, 0, SDL_LOGICAL_PRESENTATION_DISABLED);

	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

void UI::DrawUI(float deltaTime)
{
	ImGui::Begin("Settings");

	ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
	ImGui::Text("Delta Time: %.4f", deltaTime);

	int count = world->desiredBotCount;
	ImGui::SliderInt("Bot Count", &count, 0, 100'000);
	world->desiredBotCount = count;

	ImGui::End();
}

void UI::EndFrame()
{
	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

	SDL_SetRenderLogicalPresentation(renderer, logicalWidth, logicalHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);
}
