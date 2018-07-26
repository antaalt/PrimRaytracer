#include "Renderer.h"
#include "Ray.h"
#include "Tracer.h"
#include "Camera.h"
#include "Texture.h"
#include "Octree.h"
#include "NoAccel.h"


namespace Application {

	Renderer::Renderer() : Renderer(DEFAULT_WIDTH, DEFAULT_HEIGHT)
	{
	}


	Renderer::Renderer(unsigned int width, unsigned int height) : m_width(width), m_height(height)
	{
	}

	Renderer::~Renderer()
	{
	}

	bool Renderer::loadScene(std::string path)
	{
		
		m_scene = RayTracer::Scene::GLTF::load(path);
		/*float ratio = m_resolution.x / static_cast<float>(m_resolution.y);
		m_scene.getCurrentCamera()->setProjection(90.f / ratio, ratio);
		return res;*/
		return true;
	}

	void Renderer::inputs(Inputs & inputs)
	{
		/*Camera *camera = m_scene.getCurrentCamera();
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
		inputs.mouse.wheel.y = 0;*/
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

		glViewport(0, 0, m_width, m_height);
		glScissor(0, 0, m_width, m_height);
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

	bool Renderer::render(const RayTracer::Tracer &tracer)
	{
		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClearDepth(1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RayTracer::NoAccel acceleration; // TODO build scene
		RayTracer::Options options;
		RayTracer::Camera camera;
		options.backgroundColor = Color32();
		options.maxDepth = 10;

		RayTracer::Texture output;
		// TODO add samples
		for (unsigned int y = 0; y < m_height; y++)
		{
			for (unsigned int x = 0; x < m_width; x++)
			{
				// TODO generate ray with camera
				RayTracer::Ray ray(Point3(0.f), Vector3(0.f, 0.f, 1.f));
				RayTracer::Pixel pixel = tracer.castRay(ray, acceleration, options);
				output.set(x, y, pixel);
			}
		}

		glDrawPixels(m_width, m_height, GL_FLOAT, GL_FLOAT, output.data());

		output.saveToFile("output.jpg");

		return true;
	}
}