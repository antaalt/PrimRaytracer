#pragma once

/*#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif*/
#include <SDL.h>

#include "Config.h"
#include "Renderer.h"
#include "GUI.h"


namespace app {

	struct options {
		raycore::tracer::Tracer *tracer;
		raycore::tracer::Camera *camera;
		raycore::prim::Acceleration acceleration;
		raycore::tracer::Settings settings;
	};

	class Application
	{
	public:
		Application(unsigned int p_width, unsigned int p_height);
		~Application();
		Application(const Application &acc) = delete;
		Application& operator=(const Application &other) = delete;

		void run(raycore::prim::Scene &scene, options &options);

	private:
		// Application loop
		bool inputs(const Inputs &inputs);
		// Utils
		bool save(std::string path);
		void resize(unsigned int width, unsigned int height);

	private:
		SDL_DisplayMode m_displayMode;
		SDL_Window *m_window;
		SDL_GLContext m_glContext;
		raycore::tracer::Renderer *m_renderer;
		raycore::tracer::Camera *m_camera;
		unsigned int m_width, m_height;
		GUI m_gui;
	};

}