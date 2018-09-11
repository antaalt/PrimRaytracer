#include "Application.h"

#include "Utils/Timer.h"
#include "Utils/Log.h"
#include "PathTracer.h"
#include "WhittedTracer.h"
#include "PinholeCamera.h"


namespace app {

	Application::Application() : Application(DEFAULT_WIDTH, DEFAULT_HEIGHT)
	{
	}

	Application::Application(unsigned int p_width, unsigned int p_height)
	{
		m_width = p_width;
		m_height = p_height;
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
		Log::info("Displaying window in ", m_width, "x", m_height, "px");
		// Window creation
		m_window = SDL_CreateWindow(PROJECT_NAME, 0, 0, m_width, m_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);

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

		glDrawBuffer(GL_BACK);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_TEXTURE_2D);

		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

		glViewport(0, 0, this->m_width, this->m_height);
		glScissor(0, 0, this->m_width, this->m_height);
		glEnable(GL_SCISSOR_TEST);

		// Glew initialisation
		glewExperimental = GL_TRUE;
		GLenum status = glewInit();
		if (status != GLEW_OK)
		{
			Log::error("Error while initializing GLEW : ", glewGetErrorString(status));
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
		return inputs();
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

	void Application::run(app::Scene &scene, options &options)
	{
		if (!init()) {
			Log::error("Could not init window");
			return;
		}
		Utils::Timer timer;

		tracer::Renderer renderer(m_width, m_height, 32);
		bool init = renderer.buildScene(scene, options.acceleration);
		renderer.setTracer(options.tracer);
		renderer.setCamera(options.camera);
		m_camera = options.camera;
		// This makes our buffer swap synchronized with the monitor's vertical refresh
		SDL_GL_SetSwapInterval(1);
		// LOOP		
		while (!events())
		{
			glClearColor(1.f, 1.f, 1.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);
			timer.tick();
			bool update = renderer.updateRays();
			if (update)
				renderer.renderPreview();
			else
				renderer.render();
			glDrawPixels(this->m_width, this->m_height, GL_RGBA, GL_FLOAT, renderer.image().data());
			Log::info("Duration : ", timer.elapsedTime<Utils::Timer::milliseconds>(), "ms");
			SDL_GL_SwapWindow(m_window);
			// Free the processor
			SDL_Delay(0);
		}
		m_camera = nullptr; // Application do not own the pointer
		destroy();
	}

	bool Application::inputs()
	{
		if (m_camera == nullptr)
			return true;
		const float scaleFactor = 0.01f;
		if (m_inputs.mouse.mouse[LEFT])
		{
			this->m_camera->rotate(static_cast<float>(-m_inputs.mouse.pos[0]), vec3(0.f, 1.f, 0.f));
			this->m_camera->rotate(static_cast<float>(-m_inputs.mouse.pos[1]), vec3(1.f, 0.f, 0.f));
		}
		if (m_inputs.mouse.mouse[RIGHT])
		{
			this->m_camera->translate(vec3(-m_inputs.mouse.pos[0] * 0.01f, m_inputs.mouse.pos[1] * 0.01f, 0.f));
		}
		if (m_inputs.mouse.wheel[1] != 0)
			this->m_camera->translate(vec3(0.f, 0.f, static_cast<float>(m_inputs.mouse.wheel[1])* 0.1f));
		m_inputs.mouse.pos[0] = 0;
		m_inputs.mouse.pos[1] = 0;
		m_inputs.mouse.wheel[0] = 0;
		m_inputs.mouse.wheel[1] = 0;
		return m_inputs.keyboard.escape || m_inputs.keyboard.space;
	}

	void Application::destroy()
	{
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

}