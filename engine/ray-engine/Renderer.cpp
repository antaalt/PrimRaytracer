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
			width(width), 
			height(height), 
			camera(nullptr), 
			accelerator(nullptr), 
			tracer(nullptr)
		{
			this->output = PixelBuffer(width, height);
			this->rays.resize(width *  height);
		}

		Renderer::~Renderer()
		{
			if (this->camera != nullptr)
				delete this->camera;
			if (this->accelerator != nullptr)
				delete this->accelerator;
			if (this->tracer != nullptr)
				delete this->tracer;
		}

		bool Renderer::loadScene(std::string path, Acceleration acceleration)
		{
#if 0
			this->scene = SceneBuilder::buildCustomSceneTri();
			this->accelerator = new NoAccel();
			return this->accelerator->build(this->scene);
#else
			this->scene = Scene::GLTF::load(path);
			switch (acceleration)
			{
			case Acceleration::ACCELERATION_OCTREE:
				this->accelerator = new Octree();
				break;
			case Acceleration::NO_ACCEL:
				this->accelerator = new NoAccel();
				break;
			case Acceleration::ACCELERATION_BOUNDING_BOX:
			default:
				return false;
			}
			return this->accelerator->build(this->scene);
#endif
		}

		void Renderer::inputs(Inputs & inputs)
		{
			if (this->camera == nullptr)
				return;
			const float scaleFactor = 0.01f;
			if (inputs.mouse.mouse[LEFT])
			{
				this->camera->rotate(static_cast<float>(-inputs.mouse.pos[0]), Vector3(0.f, 1.f, 0.f));
				this->camera->rotate(static_cast<float>(-inputs.mouse.pos[1]), Vector3(1.f, 0.f, 0.f));
			}
			if (inputs.mouse.mouse[RIGHT])
			{
				this->camera->translate(Vector3(-inputs.mouse.pos[0] * 0.01f, inputs.mouse.pos[1] * 0.01f, 0.f));
			}
			if(inputs.mouse.wheel[1] != 0)
				this->camera->translate(Vector3(0.f, 0.f, static_cast<float>(inputs.mouse.wheel[1])* 0.1f));
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

			glViewport(0, 0, this->width, this->height);
			glScissor(0, 0, this->width, this->height);
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

		bool Renderer::updateRays()
		{
			unsigned int index = 0;
			if (!this->camera->computeTransform())
				return false;
			for (unsigned int y = 0; y < this->height; y++)
				for (unsigned int x = 0; x < this->width; x++)
					this->rays[index++] = this->camera->generateRay(x, y);
			return true;
		}

		bool Renderer::renderPreview()
		{
			// TODO reduce depth for this render
			glClearColor(1.f, 1.f, 1.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);

			Log::debug("Rendering preview");

			Pixel *pixel = this->output.data();
			unsigned int tileWidth = this->width / TILE_WIDTH_NUMBER;
			unsigned int tileHeight = this->height / TILE_HEIGHT_NUMBER;
			for (unsigned int y = 0; y < TILE_HEIGHT_NUMBER; y++)
			{
				for (unsigned int x = 0; x < TILE_WIDTH_NUMBER; x++)
				{
					const unsigned int indexY = y * tileHeight * this->width;
					const unsigned int indexX = x * tileWidth;
					Pixel outPixel = this->tracer->castRay(this->rays[indexY + indexX], this->accelerator);

					for (unsigned int yTile = indexY; yTile < indexY + tileHeight * this->width; yTile += this->width)
						for (unsigned int xTile = indexX; xTile < indexX + tileWidth; xTile++)
							pixel[yTile + xTile] = outPixel;
				}
			}
			glDrawPixels(this->width, this->height, GL_RGBA, GL_FLOAT, this->output.data());

			return true;
		}

		bool Renderer::render()
		{
			glClearColor(1.f, 1.f, 1.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);

			Log::debug("Rendering");

			// TODO add samples
			Pixel *pixel = this->output.data();
			unsigned int index = 0;
			for (unsigned int y = 0; y < this->height; y++)
			{
				for (unsigned int x = 0; x < this->width; x++, index++)
				{
					if ((y == this->height / 2.f) && (x == this->width / 2.f))
					{
						Log::info(this->rays[index].direction, " - ", this->rays[index].origin);
					}
					pixel[index] = this->tracer->castRay(this->rays[index], this->accelerator); // TODO average by samples
				}
				//Log::debug("Progress : ", (y / static_cast<float>(this->height)) * 100.f, "%");
			}
			glDrawPixels(this->width, this->height, GL_RGBA, GL_FLOAT, this->output.data());

			return true;
		}
		void Renderer::setTracer(tracer::Tracer::Ptr tracer)
		{
			this->tracer = tracer;
		}
		void Renderer::setCamera(tracer::Camera::Ptr camera)
		{
			this->camera = camera;
			// TODO, manage projection, hide camera impementation
			/*float ratio = this->resolution.x / static_cast<float>(this->resolution.y);
			this->scene.getCurrentCamera()->setProjection(90.f / ratio, ratio);
			return res;*/
		}
	}
}