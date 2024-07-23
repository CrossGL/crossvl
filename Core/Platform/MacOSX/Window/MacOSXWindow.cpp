#include "MacOSXWindow.h"

#include <SDL2/SDL.h>

namespace CGL::Core::Platform::MacOSX
{
	namespace
	{
		SDL_Window* window;
		SDL_Event evnt;
	}
	
	Window::Window(std::string_view title, u16 width, u16 height) : is_open{ false }
	{
		if (SDL_InitSubSystem(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
		{
			SDL_Log("%s", SDL_GetError());
			exit(-1);
		}

		SDL_Window* window = SDL_CreateWindow(
			"Visualizer",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width, height,
			SDL_WINDOW_METAL
		);

		if (!window)
		{
			SDL_Log("%s", SDL_GetError());
			SDL_Quit();
			exit(-1);
		}

		this->is_open = true;
	}

	Window::~Window()
	{

	}

	void Window::Show()
	{
		SDL_ShowWindow(window);
	}
	
	void Window::Update()
	{
		while (SDL_PollEvent(&evnt))
        {
            // ImGui_ImplSDL2_ProcessEvent(&evnt);
            if (evnt.type == SDL_QUIT || evnt.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                is_open = SDL_FALSE;

		}

		// metal calls
	}

	void Window::Close()
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void Window::ProcessMessages()
	{
		
	}

}
