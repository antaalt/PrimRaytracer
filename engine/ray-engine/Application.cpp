#include "Application.h"

#include "Utils/Timer.h"
#include "Utils/Log.h"
#include "PathTracer.h"


namespace Application {

	Application::Application() : Application(DEFAULT_WIDTH, DEFAULT_HEIGHT)
	{
	}

	Application::Application(unsigned int p_width, unsigned int p_height)
	{
		m_windowSize.w = p_width;
		m_windowSize.h = p_height;
	}

	Application::~Application()
	{
	}

	bool Application::init()
	{
		// SDL Initialisation
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			Log::error("Error during the SDL initialisation", SDL_GetError());
			SDL_Quit();
			return false;
		}
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY /*SDL_GL_CONTEXT_PROFILE_CORE*/);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		// Depth of the double buffering (bit)
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		// Get display resolution and manage multi screen
		for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i) {
			int should_be_zero = SDL_GetCurrentDisplayMode(i, &m_displayMode);
			if (should_be_zero != 0)
				Log::error("Could not get display mode for video display ", i, ": ", SDL_GetError());
			else
				Log::info("Display ", i, ": current display mode is ", m_displayMode.w, "x", m_displayMode.h, "px @ ", m_displayMode.refresh_rate, "hz.");
		}
		Log::info("Displaying window in ", m_windowSize.w, "x", m_windowSize.h, "px");
		// Window creation
		m_window = SDL_CreateWindow(PROJECT_NAME, 0, 0, m_windowSize.w, m_windowSize.h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);

		// checking the state of the window
		if (m_window == 0)
		{
			Log::error("Error during the window creation", SDL_GetError());
			SDL_Quit();
			return false;
		}

		// OpenGL initialisation
		m_glContext = SDL_GL_CreateContext(m_window);
		if (m_glContext == 0)
		{
			Log::error(SDL_GetError());
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return false;
		}
		

		return true;
	}

	bool Application::events()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			// Event managment
			switch (e.type)
			{
			case SDL_QUIT:
				return true;
			case SDL_KEYDOWN:
				onKeyDown(e.key);
				break;
			case SDL_KEYUP:
				onKeyUp(e.key);
				break;
			case SDL_MOUSEBUTTONDOWN:
				onMouseButtonDown(e.button);
				break;
			case SDL_MOUSEBUTTONUP:
				onMouseButtonUp(e.button);
				break;
			case SDL_MOUSEMOTION:
				onMouseMotion(e.motion);
				break;
			case SDL_MOUSEWHEEL:
				onMouseWheel(e.wheel);
				break;
			default:
				break;
			}
		}
		
		return m_inputs.keyboard.escape || m_inputs.keyboard.escape;
	}

	void Application::onMouseButtonUp(SDL_MouseButtonEvent & e)
	{
		switch (e.button)
		{
		case SDL_BUTTON_LEFT:
			m_inputs.mouse.mouse[LEFT] = false;
			break;
		case SDL_BUTTON_RIGHT:
			m_inputs.mouse.mouse[RIGHT] = false;
			break;
		case SDL_BUTTON_MIDDLE:
			m_inputs.mouse.mouse[MIDDLE] = false;
			break;
		}
	}

	void Application::onMouseButtonDown(SDL_MouseButtonEvent & e)
	{
		switch (e.button)
		{
		case SDL_BUTTON_LEFT:
			m_inputs.mouse.mouse[LEFT] = true;
			break;
		case SDL_BUTTON_RIGHT:
			m_inputs.mouse.mouse[RIGHT] = true;
			break;
		case SDL_BUTTON_MIDDLE:
			m_inputs.mouse.mouse[MIDDLE] = true;
			break;
		}
	}

	void Application::onMouseMotion(SDL_MouseMotionEvent & e)
	{
		m_inputs.mouse.pos[0] += e.xrel;
		m_inputs.mouse.pos[1] += e.yrel;
	}

	void Application::onMouseWheel(SDL_MouseWheelEvent & e)
	{
		m_inputs.mouse.wheel[0] += e.x;
		m_inputs.mouse.wheel[1] += e.y;
	}

	void Application::onKeyDown(SDL_KeyboardEvent &e)
	{
		switch (e.keysym.sym)
		{
		case SDLK_SPACE:
			m_inputs.keyboard.space = true;
			break;
		case SDLK_PRINTSCREEN:
			m_inputs.keyboard.printScreen = true;
			break;
		case SDLK_ESCAPE:
			m_inputs.keyboard.escape = true;
			break;
		case SDLK_LALT:
			m_inputs.keyboard.alt[LEFT] = true;
			break;
		case SDLK_RALT:
			m_inputs.keyboard.alt[RIGHT] = true;
			break;
		case SDLK_RCTRL:
			m_inputs.keyboard.ctrl[RIGHT] = true;
			break;
		case SDLK_LCTRL:
			m_inputs.keyboard.ctrl[LEFT] = true;
			break;
		case SDLK_LSHIFT:
			m_inputs.keyboard.shift[LEFT] = true;
			break;
		case SDLK_RSHIFT:
			m_inputs.keyboard.shift[RIGHT] = true;
			break;
		}
	}

	void Application::onKeyUp(SDL_KeyboardEvent & e)
	{
		switch (e.keysym.sym)
		{
		case SDLK_SPACE:
			m_inputs.keyboard.space = false;
			break;
		case SDLK_PRINTSCREEN:
			m_inputs.keyboard.printScreen = false;
			break;
		case SDLK_ESCAPE:
			m_inputs.keyboard.escape = false;
			break;
		case SDLK_LALT:
			m_inputs.keyboard.alt[LEFT] = false;
			break;
		case SDLK_RALT:
			m_inputs.keyboard.alt[RIGHT] = false;
			break;
		case SDLK_RCTRL:
			m_inputs.keyboard.ctrl[RIGHT] = false;
			break;
		case SDLK_LCTRL:
			m_inputs.keyboard.ctrl[LEFT] = false;
			break;
		case SDLK_LSHIFT:
			m_inputs.keyboard.shift[LEFT] = false;
			break;
		case SDLK_RSHIFT:
			m_inputs.keyboard.shift[RIGHT] = false;
			break;
		}
	}

	void Application::run()
	{
		if (!init()) {
			Log::error("Could not init window");
			return;
		}
		Utils::Timer timer;

		Renderer renderer;
		bool init = renderer.init();
		ASSERT(init == true, "Renderer not initialized");
		init = renderer.loadScene("NOT_DEFINED_YET_TODO_GLTF");
		ASSERT(init == true, "Scene not loaded");
		// This makes our buffer swap synchronized with the monitor's vertical refresh
		SDL_GL_SetSwapInterval(1);

		// LOOP
		timer.tick();
		RayTracer::PathTracer tracer(10);
		while (!events())
		{
			timer.displayFPS();

			renderer.inputs(m_inputs);
			renderer.render(tracer);

			SDL_GL_SwapWindow(m_window);
			// Free the processor
			SDL_Delay(0);
		}

		destroy();
	}

	void Application::destroy()
	{
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

}