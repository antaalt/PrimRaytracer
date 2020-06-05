#pragma once


#if defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include <GLFW/glfw3.h>

#include "Config.h"
#include "Scene.h"
#include "Camera.h"
#include "PerspectiveCamera.h"

#include <vector>
#include <string>

namespace app {

enum KeyPosition {
	RIGHT,
	LEFT,
	MIDDLE
};

struct Inputs {
	struct Mouse {
		bool mouse[3];
		int relPos[2];
		int position[2];
		int wheel;
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

struct GUISettings {
	float fov;
	int depth;
	std::vector<std::string> scene;
};

class GUI
{
public:
	GUI(unsigned int width, unsigned int height);
	~GUI();

	void create(GLFWwindow *window, const char* glslVersion);
	void destroy();

	void startFrame();
	bool draw(prim::Scene &scene, prim::Camera &camera);
	void render();
	Inputs &events();

	void setProgress(size_t tileComplete, size_t tileCount);

private:
	size_t m_tileComplete, m_tileCount;
	GLFWwindow *m_window;
	unsigned int m_width, m_height;
	Inputs m_inputs;
};
}
