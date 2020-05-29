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
#include "Renderer.h"
#include "GUI.h"


namespace app {

struct options {
	raycore::tracer::Tracer *tracer;
	raycore::tracer::Camera *camera;
	raycore::tracer::Settings settings;
};

class Application
{
public:
	Application(unsigned int p_width, unsigned int p_height);
	~Application();
	Application(const Application &acc) = delete;
	Application& operator=(const Application &other) = delete;

	void run(raycore::Scene &scene, options &options);

private:
	// Application loop
	bool inputs(const Inputs &inputs);
	// Utils
	bool save(std::string path);
public:
	void resize(unsigned int width, unsigned int height);

private:
	GLFWwindow *m_window;
	raycore::tracer::Renderer *m_renderer;
	raycore::tracer::Camera *m_camera;
	unsigned int m_width, m_height;
	GUI m_gui;
};

}