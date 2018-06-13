#include "GLRenderer.h"

#include <cmath>

namespace Application {

	GLRenderer::GLRenderer() : GLRenderer(glm::uvec2(DEFAULT_WIDTH, DEFAULT_HEIGHT))
	{
	}


	GLRenderer::GLRenderer(glm::uvec2 resolution) : m_resolution(resolution)
	{
	}

	GLRenderer::~GLRenderer()
	{
	}

	bool GLRenderer::loadScene(std::string path)
	{
		return m_scene.loadScene(path);
	}

	void GLRenderer::inputs(Inputs & inputs)
	{
		World::Camera *camera = m_scene.getCurrentCamera();
		if (camera == nullptr)
			return;
		const float scaleFactor = 0.01f;
		if (inputs.mouse.mouse[LEFT])
		{
			camera->rotate(inputs.mouse.pos.x * scaleFactor, glm::vec3(0.f, 1.f, 0.f));
			camera->rotate(inputs.mouse.pos.y * scaleFactor, glm::vec3(1.f, 0.f, 0.f));
		}
		if (inputs.mouse.mouse[RIGHT])
		{
			camera->translate(glm::vec3(-scaleFactor * inputs.mouse.pos.x, scaleFactor * inputs.mouse.pos.y, 0.f));
		}
		camera->translate(glm::vec3(0.f, 0.f, -inputs.mouse.wheel.y * scaleFactor));
		//Log::debug("POSITION : " + glm::to_string(inputs.mouse.pos) + " - " + glm::to_string(inputs.mouse.wheel));
		inputs.mouse.pos.x = 0;
		inputs.mouse.pos.y = 0;
		inputs.mouse.wheel.x = 0;
		inputs.mouse.wheel.y = 0;
	}

	bool GLRenderer::init()
	{
		glDrawBuffer(GL_BACK);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		/*glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);*/

		glEnable(GL_TEXTURE_2D);

		glViewport(0, 0, m_resolution.x, m_resolution.y);
		glScissor(0, 0, m_resolution.x, m_resolution.y);
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

	bool GLRenderer::render()
	{
		return m_scene.render();
	}
}