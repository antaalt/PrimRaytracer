#include "Application.h"

#include "Config.h"
#include "Utils/Timer.h"
#include "Utils/Log.h"
#include "PathTracer.h"
#include "WhittedTracer.h"
#include "PinholeCamera.h"
#include "stb_image_write.h"


namespace app {

	Application::Application(unsigned int width, unsigned int height) :
		m_width(width),
		m_height(height),
		m_renderer(nullptr),
		m_gui(GUI(width, height))
	{
		// SDL Initialisation
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			Log::error("Error during the SDL initialisation", SDL_GetError());
			SDL_Quit();
			return;
		}
#if defined(__APPLE__)
		// GL 3.2 Core + GLSL 150
		const char* glsl_version = "#version 150";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY /*SDL_GL_CONTEXT_PROFILE_CORE*/);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

		// Create window with graphics context
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		// Get display resolution and manage multi screen
		for (int i = 0; i < SDL_GetNumVideoDisplays(); i++) {
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
			return;
		}

		// OpenGL initialisation
		m_glContext = SDL_GL_CreateContext(m_window);
		if (m_glContext == 0)
		{
			Log::error(SDL_GetError());
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return;
		}
		// This makes our buffer swap synchronized with the monitor's vertical refresh
		SDL_GL_SetSwapInterval(1); // enable vsync

								   // Glew initialisation
		glewExperimental = GL_TRUE;
		GLenum status = glewInit();
		if (status != GLEW_OK)
		{
			Log::error("Error while initializing GLEW : ", glewGetErrorString(status));
			return;
		}

		// OpenGL
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glViewport(0, 0, this->m_width, this->m_height);
		// shaders
		m_gui.build(m_window, m_glContext, glsl_version);
	}

	Application::~Application()
	{
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	bool Application::save(std::string path)
	{
		const float* data = this->m_renderer->image().data()->data;
		Log::info("Render saved at '", path, "'");
		return (stbi_write_hdr(path.c_str(), m_width, m_height, 4, data) == 0);
	}

	void Application::resize(unsigned int width, unsigned int height)
	{
		m_renderer->resize(width, height);
	}

	void Application::run(raycore::Scene &scene, options &options)
	{
		Utils::Timer timer;

		m_renderer = new raycore::tracer::Renderer(m_width, m_height, options.settings);
		bool init = m_renderer->buildScene(std::move(scene), options.acceleration);
		m_renderer->setTracer(options.tracer);
		m_renderer->setCamera(options.camera);
		m_camera = options.camera;
		// LOOP		
		while (!inputs(m_gui.events()))
		{
			Log::info("--- Rendering frame");
			glClearColor(1.f, 1.f, 1.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);

			/*bool updated;
			const GUISettings &gui = m_gui.settings(updated);
			if (updated)
			{
				raycore::Config::maxDepth = gui.depth;
			}*/
			timer.tick();
			bool update = m_renderer->updateRays();
			Log::info("Ray updated : ", timer.elapsedTime<Utils::Timer::milliseconds>(), "ms");
			
			timer.tick();
			if (update)
				m_renderer->renderPreview();
			else
				m_renderer->render();
			Log::info("Image rendered : ", timer.elapsedTime<Utils::Timer::milliseconds>(), "ms");
			
			timer.tick();
			glDrawPixels(this->m_width, this->m_height, GL_RGBA, GL_FLOAT, m_renderer->image().data());
			Log::info("Image sent to GPU : ", timer.elapsedTime<Utils::Timer::milliseconds>(), "ms");
			
			timer.tick();
			m_gui.draw();
			Log::info("GUI drawn : ", timer.elapsedTime<Utils::Timer::milliseconds>(), "ms");
			timer.tick();
			SDL_GL_SwapWindow(m_window);
			// Free the processor
			SDL_Delay(0);
		}
		m_camera = nullptr; // Application do not own the pointer
	}

	bool Application::inputs(const Inputs &inputs)
	{
		/*if(inputs.resized)
			resize()*/
		if (m_camera == nullptr)
			return true;
		const float scaleFactor = 0.01f;
		if (inputs.mouse.mouse[LEFT])
		{
			this->m_camera->rotate(static_cast<float>(-inputs.mouse.relPos[0]), vec3(0.f, 1.f, 0.f));
			this->m_camera->rotate(static_cast<float>(-inputs.mouse.relPos[1]), vec3(1.f, 0.f, 0.f));
		}
		if (inputs.mouse.mouse[RIGHT])
			this->m_camera->translate(vec3(-inputs.mouse.relPos[0] * 0.01f, inputs.mouse.relPos[1] * 0.01f, 0.f));
		if (inputs.mouse.wheel[1] != 0)
			this->m_camera->translate(vec3(0.f, 0.f, static_cast<float>(inputs.mouse.wheel[1])* 0.1f));
		if (inputs.keyboard.printScreen)
			this->save("output.jpg");
		return inputs.keyboard.escape || inputs.keyboard.space;
	}

}