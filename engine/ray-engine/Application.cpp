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
}

Application::~Application()
{
	m_gui.destroy();
	glFinish();
	glfwTerminate();
}

bool Application::save(std::string path)
{
	const float* data = this->m_renderer->image().data()->data;
	std::vector<unsigned char> output(m_width * m_height * 3);
	for (unsigned int y = 0; y < m_height; y++)
		for (unsigned int x = 0; x < m_width; x++)
			for (unsigned int i = 0; i < 3; i++)
				output[y * m_width * 3 + x * 3 + i] = static_cast<unsigned char>(geometry::clamp(data[y * m_width * 4 + x * 4 + i], 0.f, 1.f) * 255.f);
	stbi_flip_vertically_on_write(true);
	int save = stbi_write_jpg(path.c_str(), m_width, m_height, 3, output.data(), 100);
	Log::info("Render saved at '", path, "'");
	return save == 0;
}

void Application::resize(unsigned int width, unsigned int height)
{
	m_renderer->resize(width, height);
	m_width = width;
	m_height = height;
	glViewport(0, 0, width, height);
}

void Application::run(raycore::Scene &scene, options &options)
{
	Utils::StopWatch<> stopWatch;

	m_renderer = new raycore::tracer::Renderer(m_width, m_height, options.settings);
	scene.build();
	m_renderer->setScene(std::move(scene));
	m_renderer->setTracer(options.tracer);
	m_renderer->setCamera(options.camera);
	m_camera = options.camera;
	// LOOP
	do
	{
		Log::info("--- Rendering frame");
		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_gui.startFrame();
		inputs(m_gui.events());
		m_gui.draw();

		stopWatch.start();
		bool update = m_renderer->updateRays();
		Log::info("Ray updated : ", stopWatch.elapsed(), "ms");
			
		stopWatch.start();
		if (update)
			m_renderer->renderPreview();
		else
			m_renderer->render();
		Log::info("Image rendered : ", stopWatch.elapsed(), "ms");
			
		stopWatch.start();
		glDrawPixels(this->m_width, this->m_height, GL_RGBA, GL_FLOAT, m_renderer->image().data());
		glFinish();
		Log::info("Image sent to GPU : ", stopWatch.elapsed(), "ms");
			
		stopWatch.start();
		m_gui.render();
		Log::info("GUI drawn : ", stopWatch.elapsed(), "ms");

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	} while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(m_window) == 0);
	save("output.jpg");
	m_camera = nullptr; // Application do not own the pointer
}

bool Application::inputs(const Inputs &inputs)
{
	if (m_camera == nullptr)
		return true;
	const float scaleFactor = 0.01f;
	if (inputs.mouse.mouse[LEFT])
	{
		this->m_camera->rotate(vec3f(0.f, 1.f, 0.f), geometry::degreef(static_cast<float>(-inputs.mouse.relPos[0])));
		this->m_camera->rotate(vec3f(1.f, 0.f, 0.f), geometry::degreef(static_cast<float>(-inputs.mouse.relPos[1])));
	}
	if (inputs.mouse.mouse[RIGHT])
		this->m_camera->translate(vec3f(-inputs.mouse.relPos[0] * 0.01f, inputs.mouse.relPos[1] * 0.01f, 0.f));
	if (inputs.mouse.wheel != 0)
		this->m_camera->translate(vec3f(0.f, 0.f, static_cast<float>(inputs.mouse.wheel)* 0.1f));
	if (inputs.keyboard.printScreen)
		this->save("output.jpg");
	return inputs.keyboard.escape || inputs.keyboard.space;
}

}