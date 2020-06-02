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

enum class CameraType {
	PERSPECTIVE,
	UNDEFINED
};

enum class HitableType {
	MESH,
	MESH_BVH,
	MESH_OCTREE,
	SPHERE,
	TRIANGLE,
	UNDEFINED
};
enum class MaterialType {
	MATTE,
	METAL,
	GLASS,
	UNDEFINED
};
enum class TextureType {
	CONSTANT,
	CHECKER,
	IMAGE,
	IMAGE_HDR,
	UNDEFINED
};

CameraType getType(const prim::Camera &camera)
{
	if (nullptr != dynamic_cast<const prim::PerspectiveCamera*>(&camera))
		return CameraType::PERSPECTIVE;
	return CameraType::UNDEFINED;
}

HitableType getType(const prim::Hitable &hitable)
{
	if (nullptr != dynamic_cast<const prim::Mesh*>(&hitable))
		return HitableType::MESH;
	else if (nullptr != dynamic_cast<const prim::MeshBVH*>(&hitable))
		return HitableType::MESH_BVH;
	else if (nullptr != dynamic_cast<const prim::MeshOctree*>(&hitable))
		return HitableType::MESH_OCTREE;
	else if (nullptr != dynamic_cast<const prim::Sphere*>(&hitable))
		return HitableType::SPHERE;
	else if (nullptr != dynamic_cast<const prim::Triangle*>(&hitable))
		return HitableType::TRIANGLE;
	return HitableType::UNDEFINED;
}

MaterialType getType(const prim::Material &material)
{
	if (nullptr != dynamic_cast<const prim::Matte*>(&material))
		return MaterialType::MATTE;
	else if (nullptr != dynamic_cast<const prim::Metal*>(&material))
		return MaterialType::METAL;
	else if (nullptr != dynamic_cast<const prim::Glass*>(&material))
		return MaterialType::GLASS;
	return MaterialType::UNDEFINED;
}

TextureType getType(const prim::Texture<float> &texture)
{
	if (nullptr != dynamic_cast<const prim::TextureMapFloat*>(&texture))
		return TextureType::IMAGE_HDR;
	else if (nullptr != dynamic_cast<const prim::TextureMapFloat32*>(&texture))
		return TextureType::IMAGE;
	else if (nullptr != dynamic_cast<const prim::Checker<float>*>(&texture))
		return TextureType::CHECKER;
	else if (nullptr != dynamic_cast<const prim::ConstantTexture<float>*>(&texture))
		return TextureType::CONSTANT;
	return TextureType::UNDEFINED;
}

bool GUI::draw(prim::Scene &scene, prim::Camera &camera)
{
	bool needUpdate = false;
	if (ImGui::Begin("Prim"))
	{
		if (ImGui::CollapsingHeader("Settings"))
		{
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			static int depth = 10;
			ImGui::SliderInt("Depth", &depth, 0, 15);
		}
		if (ImGui::CollapsingHeader("Camera"))
		{
			CameraType type = getType(camera);
			if (type == CameraType::PERSPECTIVE)
			{
				prim::PerspectiveCamera &pCamera = dynamic_cast<prim::PerspectiveCamera&>(camera);
				static float zNear = 0.1f;
				static float zFar = 1000.f;
				bool projUpdated = false;
				projUpdated |= ImGui::SliderFloat("Field of view", &pCamera.hFov, 1.0f, 180.0f);
				projUpdated |= ImGui::SliderFloat("Near", &zNear, 0.001f, zFar - 0.1f);
				projUpdated |= ImGui::SliderFloat("Far", &zFar, zNear + 0.1f, 1000.0f);
				if (projUpdated)
				{
					pCamera.perspective = geometry::mat4f::perspective(geometry::degreef(pCamera.hFov), m_width / (float)m_height, zNear, zFar);
					needUpdate = true;
				}
			}
			
			ImGui::Text("Transform");
			geometry::mat4f &mat = camera.transform;
			needUpdate |= ImGui::InputFloat4("##transformcol1", mat.cols[0].data);
			needUpdate |= ImGui::InputFloat4("##transformcol2", mat.cols[1].data);
			needUpdate |= ImGui::InputFloat4("##transformcol3", mat.cols[2].data);
			needUpdate |= ImGui::InputFloat4("##transformcol4", mat.cols[3].data);
			if (ImGui::Button("Identity"))
			{
				needUpdate = true;
				camera.transform = geometry::mat4f::identity();
			}
		}
		if (ImGui::CollapsingHeader("Scene"))
		{
			if (ImGui::TreeNode("Hitables", "Hitables (%d)", scene.hitables.size()))
			{
				for (prim::Hitable *hitable : scene.hitables)
				{
					HitableType type = getType(*hitable);
					switch (type)
					{
					case HitableType::MESH:
					case HitableType::MESH_BVH:
					case HitableType::MESH_OCTREE:
						ImGui::Text("Mesh");
						break;
					case HitableType::SPHERE:
						ImGui::Text("Sphere");
						break;
					default:
						ImGui::Text("Nothing here");
						break;
					}
					ImGui::Separator();
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Materials", "Materials (%d)", scene.hitables.size()))
			{
				for (prim::Material *material : scene.materials)
				{
					MaterialType type = getType(*material);
					switch (type)
					{
					case MaterialType::MATTE:
						ImGui::Text("Matte");
						break;
					case MaterialType::METAL:
						ImGui::Text("Metal");
						break;
					case MaterialType::GLASS:
						ImGui::Text("Glass");
						break;
					default:
						ImGui::Text("Nothing here");
						break;
					}
					ImGui::Separator();
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Textures", "Textures (%d)", scene.textures.size()))
			{
				for (prim::Texture<float> *texture : scene.textures)
				{
					TextureType type = getType(*texture);
					switch (type)
					{
					case TextureType::IMAGE:
						ImGui::Text("Image");
						break;
					case TextureType::IMAGE_HDR:
						ImGui::Text("ImageHDR");
						break;
					case TextureType::CHECKER:
						ImGui::Text("Checker");
						break;
					case TextureType::CONSTANT:
						ImGui::Text("Constant");
						break;
					default:
						ImGui::Text("Nothing here");
						break;
					}
					ImGui::Separator();
				}
				ImGui::TreePop();
			}
		}

		ImGui::SameLine();
	}
	ImGui::End();
	return needUpdate;
}

void GUI::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool isKeyDown(ImGuiIO &io, int key)
{
	return io.KeysDown[key] || io.KeysDownDuration[key] > 0.f && !io.WantCaptureKeyboard;
}
bool isMouseDown(ImGuiIO &io, int key)
{
	return io.MouseDown[key] && !io.WantCaptureMouse;
}

Inputs &GUI::events()
{
	ImGuiIO& io = ImGui::GetIO();

	// Key down
	m_inputs.keyboard.escape = isKeyDown(io, GLFW_KEY_ESCAPE);
	m_inputs.keyboard.space = isKeyDown(io, GLFW_KEY_ESCAPE);
	m_inputs.keyboard.printScreen = isKeyDown(io, GLFW_KEY_PRINT_SCREEN);
	m_inputs.keyboard.escape = isKeyDown(io, GLFW_KEY_ESCAPE);
	m_inputs.keyboard.alt[LEFT] = isKeyDown(io, GLFW_KEY_LEFT_ALT);
	m_inputs.keyboard.alt[RIGHT] = isKeyDown(io, GLFW_KEY_RIGHT_ALT);
	m_inputs.keyboard.space = isKeyDown(io, GLFW_KEY_SPACE);
	m_inputs.keyboard.ctrl[RIGHT] = isKeyDown(io, GLFW_KEY_RIGHT_CONTROL);
	m_inputs.keyboard.ctrl[LEFT] = isKeyDown(io, GLFW_KEY_LEFT_CONTROL);
	m_inputs.keyboard.shift[LEFT] = isKeyDown(io, GLFW_KEY_LEFT_SHIFT);
	m_inputs.keyboard.shift[RIGHT] = isKeyDown(io, GLFW_KEY_RIGHT_SHIFT);

	// Mouse button down
	m_inputs.mouse.mouse[LEFT] = isMouseDown(io, GLFW_MOUSE_BUTTON_1);
	m_inputs.mouse.mouse[RIGHT] = isMouseDown(io, GLFW_MOUSE_BUTTON_2);
	m_inputs.mouse.mouse[MIDDLE] = isMouseDown(io, GLFW_MOUSE_BUTTON_3);

	// Mouse motion
	m_inputs.mouse.relPos[0] = (int)io.MouseDelta.x;
	m_inputs.mouse.relPos[1] = (int)io.MouseDelta.y;
	m_inputs.mouse.position[0] = (int)io.MousePos.x;
	m_inputs.mouse.position[1] = (int)io.MousePos.x;

	// Mouse wheel
	m_inputs.mouse.wheel = (int)io.MouseWheel;

	return m_inputs;
}

}