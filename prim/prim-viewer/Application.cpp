#include "Application.h"

#include "Config.h"
#include "StopWatch.h"
#include "PathTracer.h"
#include "WhittedTracer.h"
#include "PinholeCamera.h"
#include "stb_image_write.h"

namespace app {

void errorCallback(int error, const char* description)
{
	Log::error("[GLFW][", error, "] ", description);
}

void onWindowResize(GLFWwindow* window, int width, int height)
{
	Application *w = static_cast<Application *>(glfwGetWindowUserPointer(window));
	w->resize(width, height);
}

Application::Application(unsigned int width, unsigned int height) :
	m_width(width),
	m_height(height),
	m_renderer(nullptr),
	m_gui(GUI(width, height))
{
	glfwSetErrorCallback(errorCallback);
	if (glfwInit() != GLFW_TRUE)
		throw std::runtime_error("Could not init GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

#if !defined(__APPLE__)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#else
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	m_window = glfwCreateWindow(width, height, "Raycore", NULL, NULL);

	if (m_window == NULL) {
		glfwTerminate();
		throw std::runtime_error("Could not init window");
	}
	glfwSetWindowUserPointer(m_window, this);
	glfwMakeContextCurrent(m_window); // Initialise GLEW
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSwapInterval(0); // enable vsync
	glfwSetWindowSizeCallback(m_window, onWindowResize);
#if !defined(__APPLE__)
	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		Log::error("Error while initializing GLEW : ", glewGetErrorString(status));
		throw std::runtime_error("Could not init GLEW");
	}
#endif
	// OpenGL
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glViewport(0, 0, this->m_width, this->m_height);
	// shaders
	float  glLanguageVersion;
	sscanf((char *)glGetString(GL_SHADING_LANGUAGE_VERSION), "%f", &glLanguageVersion);
	GLuint version = (GLuint)(100.f * glLanguageVersion);
	char versionString[16];
	snprintf(versionString, 16, "#version %d\n", version);
	m_gui.create(m_window, versionString);
	m_output.resize(width * height);
}

Application::~Application()
{
	m_gui.destroy();
	glFinish();
	glfwTerminate();
	delete m_renderer;
}

bool Application::save(std::string path)
{
	const color4f* data = m_output.data();
	std::vector<unsigned char> output(m_width * m_height * 3);
	for (unsigned int y = 0; y < m_height; y++)
	{
		for (unsigned int x = 0; x < m_width; x++)
		{
			output[y * m_width * 3 + x * 3 + 0] = static_cast<unsigned char>(geometry::clamp(data[y * m_width + x].r, 0.f, 1.f) * 255.f);
			output[y * m_width * 3 + x * 3 + 1] = static_cast<unsigned char>(geometry::clamp(data[y * m_width + x].g, 0.f, 1.f) * 255.f);
			output[y * m_width * 3 + x * 3 + 2] = static_cast<unsigned char>(geometry::clamp(data[y * m_width + x].b, 0.f, 1.f) * 255.f);
		}
	}
	stbi_flip_vertically_on_write(true);
	int save = stbi_write_jpg(path.c_str(), m_width, m_height, 3, output.data(), 100);
	Log::info("Render saved at '", path, "'");
	return save == 0;
}

void Application::resize(unsigned int width, unsigned int height)
{
	m_output.resize(width * height);
	m_renderer->resize(width, height);
	m_width = width;
	m_height = height;
	glViewport(0, 0, width, height);
}

void Application::run(prim::Scene &scene, prim::Camera &camera, prim::Tracer &tracer)
{
	prim::StopWatch<> stopWatch;

	m_renderer = new prim::Renderer(tracer, m_width, m_height);
	scene.build();
	// LOOP
	do
	{
		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		stopWatch.start();

		m_gui.startFrame();
		bool inputsUpdated = inputs(camera, m_gui.events());
		bool sceneUpdated = m_gui.draw(scene, camera);
		if (inputsUpdated || sceneUpdated)
		{
			m_renderer->reset();
		}
			
		if (m_renderer->isWaiting())
		{
			Log::info("Launching render");
			m_renderer->getOutput(m_output.data(), 0, m_output.size());
			m_renderer->launch(camera, scene);
		}
		
		glDrawPixels(this->m_width, this->m_height, GL_RGBA, GL_FLOAT, m_output.data());
			
		m_gui.render();

		Log::info("Frame finished : ", stopWatch.elapsed(), "ms");

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	} while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(m_window) == 0);
	save("output.jpg");
}

bool Application::inputs(prim::Camera &camera, const Inputs &inputs)
{
	bool needUpdate = false;
	const float scaleFactor = 0.01f;
	if (inputs.mouse.mouse[LEFT])
	{
		needUpdate = true;
		camera.transform *= geometry::mat4f::rotate(vec3f(0.f, 1.f, 0.f), geometry::degreef(static_cast<float>(-inputs.mouse.relPos[0])));
		camera.transform *= geometry::mat4f::rotate(vec3f(1.f, 0.f, 0.f), geometry::degreef(static_cast<float>(-inputs.mouse.relPos[1])));
	}
	if (inputs.mouse.mouse[RIGHT])
	{
		needUpdate = true;
		camera.transform *= geometry::mat4f::translate(vec3f(-inputs.mouse.relPos[0] * 0.01f, inputs.mouse.relPos[1] * 0.01f, 0.f));
	}
	if (inputs.mouse.wheel != 0)
	{
		needUpdate = true;
		camera.transform *= geometry::mat4f::translate(vec3f(0.f, 0.f, static_cast<float>(inputs.mouse.wheel)* 0.1f));
	}
	if (inputs.keyboard.printScreen)
		this->save("output.jpg");
	return needUpdate;
}

}