#include "GUI.h"
#include "Config.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl.h"
#include "imgui\imgui_impl_opengl3.h"

namespace app {

	GUI::GUI(unsigned int width, unsigned int height) :
		width(width),
		height(height)
		//changed(false)
	{
		// Imgui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableKeyboard;
		/*m_settings.depth = 10;
		m_settings.fov = 90.f;*/
	}


	void GUI::build(SDL_Window * window, SDL_GLContext glContext, const char * glslVersion)
	{
		this->window = window;
		ImGui_ImplSDL2_InitForOpenGL(window, glContext);
		ImGui_ImplOpenGL3_Init(glslVersion);

		// Setup style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Read 'misc/fonts/README.txt' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != NULL);
	}
	void GUI::prepare()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(this->window);
		ImGui::NewFrame();

		{

			ImGui::Begin("RayCore - Debug window");

			ImGui::Text("Debug console for raycore");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::SliderFloat("Field of view", &raycore::Config::fov/*m_settings.fov*/, 1.0f, 180.0f);
			ImGui::SliderInt("Depth", &raycore::Config::maxDepth/*m_settings.depth*/, 0, GLOBAL_MAX_DEPTH);

			ImGui::SameLine();
			/*if (ImGui::Button("Apply"))
			{
				changed = true;
			}*/
			// check bounds
			ImVec2 size = ImGui::GetWindowSize();
			if (size.x > width || size.y > height)
				ImGui::SetWindowSize(ImVec2(min(size.x, static_cast<float>(width)), min(size.y, static_cast<float>(height))));

			ImVec2 pos = ImGui::GetWindowPos();
			if (pos.x < 0.f || pos.y < 0.f)
				ImGui::SetWindowPos(ImVec2(max(pos.x, 0.f), max(pos.y, 0.f)));
			if (pos.x + size.x > width || pos.y + size.y > height)
				ImGui::SetWindowPos(ImVec2(min(pos.x, width - size.x), min(pos.y, height - size.y)));

			// check mouse position
			if (m_inputs.mouse.position[0] > pos.x &&
				m_inputs.mouse.position[0] < pos.x + size.x &&
				m_inputs.mouse.position[1] > pos.y &&
				m_inputs.mouse.position[1] < pos.y + size.y)
			{
				m_inputs.mouse.relPos[0] = 0;
				m_inputs.mouse.relPos[1] = 0;
				/*m_inputs.mouse.mouse[0] = false;
				m_inputs.mouse.mouse[1] = false;
				m_inputs.mouse.mouse[2] = false;*/
			}

			ImGui::End();
		}

		// 3. Show another simple window.
		/*if (secondWindow)
		{
			ImGui::Begin("Another Window", &secondWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
			secondWindow = false;
			ImGui::End();
		}*/
	}

	GUI::~GUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void GUI::draw()
	{
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	Inputs &GUI::events()
	{
		SDL_Event e;
		m_inputs.mouse.relPos[0] = 0;
		m_inputs.mouse.relPos[1] = 0;
		m_inputs.mouse.wheel[0] = 0;
		m_inputs.mouse.wheel[1] = 0;
		while (SDL_PollEvent(&e))
		{
			//ImGui_ImplSDL2_ProcessEvent(&e);
			// Event managment
			switch (e.type)
			{
			case SDL_QUIT:
				m_inputs.keyboard.escape = true;
				return m_inputs;
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
			case SDL_WINDOWEVENT:
				switch (e.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					m_inputs.resized = true;
					/*e.window.windowID, e.window.data1,
					e.window.data2);*/
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
		prepare();
		return m_inputs;
	}

	void GUI::onKeyDown(SDL_KeyboardEvent & e)
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

	void GUI::onKeyUp(SDL_KeyboardEvent & e)
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

	void GUI::onMouseButtonDown(SDL_MouseButtonEvent & e)
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

	void GUI::onMouseButtonUp(SDL_MouseButtonEvent & e)
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

	void GUI::onMouseMotion(SDL_MouseMotionEvent & e)
	{
		m_inputs.mouse.relPos[0] += e.xrel;
		m_inputs.mouse.relPos[1] += e.yrel;
		m_inputs.mouse.position[0] = e.x;
		m_inputs.mouse.position[1] = e.y;
	}

	void GUI::onMouseWheel(SDL_MouseWheelEvent & e)
	{
		m_inputs.mouse.wheel[0] += e.x;
		m_inputs.mouse.wheel[1] += e.y;
	}

}