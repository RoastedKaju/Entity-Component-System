#ifndef UI_HPP
#define UI_HPP

#include "Common.hpp"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

class UI
{
public:
	UI(SDL_Window *window, SDL_Renderer *renderer);
	~UI();

	void PollEvent(SDL_Event &event);
	void StartFrame();
	void DrawUI(float deltaTime);
	void EndFrame();

private:
	SDL_Window *window;
	SDL_Renderer *renderer;

	int logicalWidth;
	int logicalHeight;
};

#endif // !UI_HPP
