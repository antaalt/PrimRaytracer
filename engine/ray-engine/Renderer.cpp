#include "Renderer.h"
#include "Ray.h"
#include "Tracer.h"
#include "Camera.h"
#include "Octree.h"
#include "NoAccel.h"

#include "SceneBuilder.h"


namespace app {
	namespace tracer {

		Renderer::Renderer(unsigned int width, unsigned int height) : 
			m_width(width), 
			m_height(height), 
			m_camera(nullptr), 
			m_accelerator(nullptr), 
			m_tracer(nullptr)
		{
			m_output = PixelBuffer(width, height);
			m_rays.resize(width *  height);
		}

		Renderer::~Renderer()
		{
			if (m_camera != nullptr)
				delete m_camera;
			if (m_accelerator != nullptr)
				delete m_accelerator;
			if (m_tracer != nullptr)
				delete m_tracer;
		}

		bool Renderer::loadScene(std::string path, Acceleration acceleration)
		{
#if 1
			m_scene = SceneBuilder::buildCustomScene();
			m_accelerator = new NoAccel();
			return m_accelerator->build(m_scene);
#else
			m_scene = Scene::GLTF::load(path);
			switch (acceleration)
			{
			case Acceleration::ACCELERATION_OCTREE:
				m_accelerator = new Octree();
				break;
			case Acceleration::NO_ACCEL:
				m_accelerator = new NoAccel();
				break;
			case Acceleration::ACCELERATION_BOUNDING_BOX:
			default:
				return false;
			}
			return m_accelerator->build(m_scene);
#endif
		}

		void Renderer::inputs(Inputs & inputs)
		{
			if (m_camera == nullptr)
				return;
			const float scaleFactor = 0.01f;
			if (inputs.mouse.mouse[LEFT])
			{
				m_camera->rotate(inputs.mouse.pos[0] * scaleFactor, Vector3(0.f, 1.f, 0.f));
				m_camera->rotate(inputs.mouse.pos[1] * scaleFactor, Vector3(1.f, 0.f, 0.f));
			}
			if (inputs.mouse.mouse[RIGHT])
			{
				m_camera->translate(Vector3(-scaleFactor * inputs.mouse.pos[0], scaleFactor * inputs.mouse.pos[1], 0.f));
			}
			m_camera->translate(Vector3(0.f, 0.f, -inputs.mouse.wheel[1] * scaleFactor));
			inputs.mouse.pos[0] = 0;
			inputs.mouse.pos[1] = 0;
			inputs.mouse.wheel[0] = 0;
			inputs.mouse.wheel[1] = 0;
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

		bool Renderer::render()
		{
			glClearColor(1.f, 1.f, 1.f, 1.f);
			glClearDepth(1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			tracer::Options options; // TODO move elsewhere
			options.backgroundColor = Pixel(0.f, 0.f, 1.f, 1.f);
			options.maxDepth = 10;
			Log::debug("Rendering");

			// TODO add samples
			Pixel *pixel = m_output.data();
			unsigned int index = 0;

			if (m_camera->computeTransform())
			{
				for (unsigned int y = 0; y < m_height; y++)
				{
					for (unsigned int x = 0; x < m_width; x++)
					{
						m_rays[index++] = m_camera->generateRay(x, y);
					}
				}
			}
			index = 0;
			for (unsigned int y = 0; y < m_height; y++)
			{
				for (unsigned int x = 0; x < m_width; x++, index++)
				{
					pixel[index] = m_tracer->castRay(m_rays[index], m_accelerator, options); // TODO average by samples
				}
				//Log::debug("Progress : ", (y / static_cast<float>(m_height)) * 100.f, "%");
			}
			glDrawPixels(m_width, m_height, GL_RGBA, GL_FLOAT, m_output.data());

			return true;
		}
		void Renderer::setTracer(tracer::Tracer::Ptr tracer)
		{
			m_tracer = tracer;
		}
		void Renderer::setCamera(tracer::Camera::Ptr camera)
		{
			m_camera = camera;
			// TODO, manage projection, hide camera impementation
			/*float ratio = m_resolution.x / static_cast<float>(m_resolution.y);
			m_scene.getCurrentCamera()->setProjection(90.f / ratio, ratio);
			return res;*/
		}
	}
}