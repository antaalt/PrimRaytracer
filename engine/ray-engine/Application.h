#pragma once

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
#include <SDL.h>

#include "Config.h"
#include "Renderer.h"



namespace app {

	struct options {
		tracer::Tracer *tracer;
		tracer::Camera *camera;
		tracer::Acceleration acceleration;
	};

	enum KeyPosition {
		RIGHT,
		LEFT,
		MIDDLE
	};
	struct Inputs {
		struct Mouse {
			bool mouse[3];
			int pos[2];
			int wheel[2];
		} mouse;
		struct KeyBoard {
			bool ctrl[2];
			bool alt[2];
			bool shift[2];
			bool space;
			bool escape;
			bool printScreen;
		} keyboard;
	};

	class Application
	{
	public:
		Application();
		Application(unsigned int p_width, unsigned int p_height);
		~Application();

		void run(app::Scene &scene, options &options);

	private:
		bool inputs();
		void destroy();
		bool init();
		bool events();
		bool save(std::string path);
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
		tracer::Camera *m_camera;
		unsigned int m_width, m_height;
		Inputs m_inputs;
	};

}