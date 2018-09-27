#include "Renderer.h"
#include "Ray.h"
#include "Tracer.h"
#include "Camera.h"
#include "Octree.h"
#include "NoAccel.h"
#include "BVH.h"

#include "SceneBuilder.h"
#include "Mathematic.h"

#include <ppl.h>


namespace raycore {
	namespace tracer {

		Renderer::Renderer(unsigned int width, unsigned int height, unsigned int tileSize) :
			width(width), 
			height(height), 
			camera(nullptr), 
			accelerator(nullptr), 
			tracer(nullptr),
			tileSize(tileSize),
			samples(0),
			output(PixelBuffer(width, height))
		{
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

		bool Renderer::buildScene(Scene &&scene, Acceleration acceleration)
		{
			switch (acceleration)
			{
			case Acceleration::OCTREE:
				this->accelerator = new Octree();
				break;
			case Acceleration::NONE:
				this->accelerator = new NoAccel();
				break;
			case Acceleration::BVH:
				this->accelerator = new BVH();
				break;
			default:
				return false;
			}
			this->buildTiles(tileSize);
			return this->accelerator->build(scene);
		}

		bool Renderer::updateRays()
		{
			if (!this->camera->computeTransform())
				return false;
			unsigned int index = 0;
			for (unsigned int y = 0; y < this->height; y++)
				for (unsigned int x = 0; x < this->width; x++)
					this->rays[index++] = this->camera->generateRay(x, y);
			this->tracer->reset();
			samples = 0;
			return true;
		}

		bool Renderer::renderPreview()
		{
			Log::debug("Rendering preview");

			Pixel *pixel = this->output.data();
			for (auto it = tiles.begin(); it != tiles.end(); it++)
			{
				Tile &tile = (*it);
				ivec2 center = tile.min + tile.center();
				const int index = center.y * this->width + center.x;
				Pixel p;
				this->tracer->castRay(p, this->rays[index], this->accelerator);
				for (int y = tile.min.y; y < tile.max.y; y++)
					for (int x = tile.min.x; x < tile.max.x; x++)
						pixel[y * this->width + x] = p;
			}
			return true;
		}

		bool Renderer::render()
		{
			Log::debug("Rendering sample ", (samples + 1));

			Pixel *pixel = this->output.data();
#if defined(PARALLEL_RENDERING)
			concurrency::parallel_for(size_t(0), tiles.size(), [&](size_t i)
			{
				Pixel *pixel = this->output.data();
				Tile &tile = this->tiles[i];
				for (int y = tile.min.y; y < tile.max.y; y++)
				{
					for (int x = tile.min.x; x < tile.max.x; x++)
					{
						const int index = y * this->width + x;
						this->tracer->castRay(pixel[index], this->rays[index], this->accelerator);
					}
				}
			});
#else
			unsigned int index = 0;
			for (unsigned int y = 0; y < this->height; y++)
			{
				for (unsigned int x = 0; x < this->width; x++, index++)
				{
					pixel[index] = this->tracer->castRay(this->rays[index], this->accelerator); // TODO average by samples
				}
			}
#endif
			samples++;
			this->tracer->postProcess();
			return true;
		}

		void Renderer::buildTiles(unsigned int tileSize) {
			ivec2 nTiles(
				static_cast<int>(std::ceil(this->width / static_cast<float>(tileSize))),
				static_cast<int>(std::ceil(this->height / static_cast<float>(tileSize)))
			);
			tiles.clear();
			for (int y = 0; y < nTiles.y; y++)
			{
				for (int x = 0; x < nTiles.x; x++)
				{
					this->tiles.push_back(Tile(
						ivec2(x * tileSize, y * tileSize),
						ivec2(min((x + 1) * tileSize, this->width), min((y + 1) * tileSize, this->height))
					));
				}
			}
		}

		void Renderer::setTracer(tracer::Tracer* tracer)
		{
			if (this->tracer != nullptr)
				delete this->tracer;
			this->tracer = tracer;
		}
		void Renderer::setCamera(tracer::Camera* camera)
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