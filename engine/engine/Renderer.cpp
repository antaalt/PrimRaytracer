#include "Renderer.h"

#include <cmath>

namespace Application {
	namespace GL {

		Renderer::Renderer() : Renderer(glm::uvec2(DEFAULT_WIDTH, DEFAULT_HEIGHT))
		{
		}


		Renderer::Renderer(glm::uvec2 resolution) : m_resolution(resolution)
		{
		}

		Renderer::~Renderer()
		{
		}

		bool Renderer::loadScene(std::string path)
		{
			bool res = m_scene.loadScene(path);
			float ratio = m_resolution.x / m_resolution.y;
			m_scene.getCurrentCamera()->setProjection(45.f / ratio, ratio);
			return res;
		}

		void Renderer::inputs(Inputs & inputs)
		{
			Camera *camera = m_scene.getCurrentCamera();
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

		bool Renderer::init()
		{
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

		bool Renderer::render()
		{
			glClearColor(1.f, 1.f, 1.f, 1.f);
			glClearDepth(1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			return m_scene.render();
		}
	}
}