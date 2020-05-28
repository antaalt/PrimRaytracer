#include "GUI.h"
#include "Config.h"

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

namespace app {

GUI::GUI(unsigned int width, unsigned int height) :
	m_width(width),
	m_height(height)
{
}

GUI::~GUI()
{
}


void GUI::create(GLFWwindow * window, const char * glslVersion)
{
	m_window = window;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	ImGui_ImplGlfw_InitForOpenGL(window, true);
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

void GUI::destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUI::startFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void GUI::draw()
{
	if (ImGui::Begin("RayCore - Debug window"))
	{

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
		if (size.x > m_width || size.y > m_height)
			ImGui::SetWindowSize(ImVec2(geometry::min(size.x, static_cast<float>(m_width)), geometry::min(size.y, static_cast<float>(m_height))));

		ImVec2 pos = ImGui::GetWindowPos();
		if (pos.x < 0.f || pos.y < 0.f)
			ImGui::SetWindowPos(ImVec2(geometry::max(pos.x, 0.f), geometry::max(pos.y, 0.f)));
		if (pos.x + size.x > m_width || pos.y + size.y > m_height)
			ImGui::SetWindowPos(ImVec2(geometry::min(pos.x, m_width - size.x), geometry::min(pos.y, m_height - size.y)));

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
	}
	ImGui::End();

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

void GUI::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Inputs &GUI::events()
{
	ImGuiIO& io = ImGui::GetIO();

	// Key down
	m_inputs.keyboard.escape = io.KeysDown[GLFW_KEY_ESCAPE];
	m_inputs.keyboard.space = io.KeysDown[GLFW_KEY_ESCAPE];
	m_inputs.keyboard.printScreen = io.KeysDown[GLFW_KEY_PRINT_SCREEN];
	m_inputs.keyboard.escape = io.KeysDown[GLFW_KEY_ESCAPE];
	m_inputs.keyboard.alt[LEFT] = io.KeysDown[GLFW_KEY_LEFT_ALT];
	m_inputs.keyboard.alt[RIGHT] = io.KeysDown[GLFW_KEY_RIGHT_ALT];
	m_inputs.keyboard.space = io.KeysDown[GLFW_KEY_SPACE];
	m_inputs.keyboard.ctrl[RIGHT] = io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	m_inputs.keyboard.ctrl[LEFT] = io.KeysDown[GLFW_KEY_LEFT_CONTROL];
	m_inputs.keyboard.shift[LEFT] = io.KeysDown[GLFW_KEY_LEFT_SHIFT];
	m_inputs.keyboard.shift[RIGHT] = io.KeysDown[GLFW_KEY_RIGHT_SHIFT];

	// Mouse button down
	m_inputs.mouse.mouse[LEFT] = io.MouseClicked[GLFW_MOUSE_BUTTON_1];
	m_inputs.mouse.mouse[RIGHT] = io.MouseClicked[GLFW_MOUSE_BUTTON_2];
	m_inputs.mouse.mouse[MIDDLE] = io.MouseClicked[GLFW_MOUSE_BUTTON_3];

	// Mouse motion
	m_inputs.mouse.relPos[0] = io.MouseDelta.x;
	m_inputs.mouse.relPos[1] = io.MouseDelta.y;
	m_inputs.mouse.position[0] = io.MousePos.x;
	m_inputs.mouse.position[1] = io.MousePos.x;

	// Mouse wheel
	m_inputs.mouse.wheel = io.MouseWheel;

	return m_inputs;
}

}