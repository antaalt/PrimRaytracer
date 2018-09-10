#include "Renderer.h"
#include "Ray.h"
#include "Tracer.h"
#include "Camera.h"
#include "Octree.h"
#include "NoAccel.h"

#include "SceneBuilder.h"

#include <ppl.h>


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

		bool Renderer::buildScene(Scene &scene, Acceleration acceleration)
		{
			this->scene = scene;
			switch (acceleration)
			{
			case Acceleration::ACCELERATION_OCTREE:
				this->accelerator = new Octree();
				break;
			case Acceleration::NO_ACCEL:
				this->accelerator = new NoAccel();
				break;
			case Acceleration::ACCELERATION_BVH:
			default:
				return false;
			}
			return this->accelerator->build(this->scene);
		}

		bool Renderer::updateRays()
		{
			if (!this->camera->computeTransform())
				return false;
			unsigned int index = 0;
			for (unsigned int y = 0; y < this->height; y++)
				for (unsigned int x = 0; x < this->width; x++)
					this->rays[index++] = this->camera->generateRay(x, y);
			return true;
		}

		bool Renderer::renderPreview()
		{
			// TODO reduce depth for this render

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

			return true;
		}

		bool Renderer::render()
		{
			Log::debug("Rendering");

			// TODO add samples
			Pixel *pixel = this->output.data();
			unsigned int index = 0;
			for (unsigned int y = 0; y < this->height; y++)
			{
				for (unsigned int x = 0; x < this->width; x++, index++)
				{
					pixel[index] = this->tracer->castRay(this->rays[index], this->accelerator); // TODO average by samples
				}
			}

			return true;
		}

		bool Renderer::renderParallel()
		{

			Log::debug("Rendering");

			// TODO add samples
			Pixel *pixel = this->output.data();
			unsigned int index = 0;

			concurrency::parallel_for(size_t(0), size_t(50), [&](size_t i)
			{
				std::cout << i << ",";
			});
			/*for (unsigned int y = 0; y < this->height; y++)
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
			}*/

			return true;
		}
		void Renderer::setTracer(tracer::Tracer::Ptr tracer)
		{
			if (this->tracer != nullptr)
				delete this->tracer;
			this->tracer = tracer;
		}
		void Renderer::setCamera(tracer::Camera::Ptr camera)
		{
			if (this->camera != nullptr)
				delete this->camera;
			this->camera = camera;
		}
		const PixelBuffer & Renderer::image() const
		{
			return output;
		}
	}
}