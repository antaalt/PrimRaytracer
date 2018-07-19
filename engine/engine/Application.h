#pragma once

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
#include <SDL.h>

#include "Config.h"
#include "Renderer.h"



namespace Application {

	class Application
	{
	public:
		Application();
		Application(unsigned int p_width, unsigned int p_height);
		~Application();

		void run();

	private:
		void destroy();
		bool init();
		bool events();
		void onKeyDown(SDL_KeyboardEvent &e);
		void onKeyUp(SDL_KeyboardEvent &e);
		void onMouseButtonDown(SDL_MouseButtonEvent &e);
		void onMouseButtonUp(SDL_MouseButtonEvent &e);
		void onMouseMotion(SDL_MouseMotionEvent &e);
		void onMouseWheel(SDL_MouseWheelEvent &e);

	private:
		SDL_DisplayMode m_displayMode;
		SDL_Window *m_window;
		SDL_GLContext m_glContext;
		SDL_Rect m_windowSize;
		Inputs m_inputs;
	};

}