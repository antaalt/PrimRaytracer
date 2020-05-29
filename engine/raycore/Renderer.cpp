#include "Renderer.h"
#include "Config.h"
#include "Ray.h"
#include "Tracer.h"
#include "Camera.h"

#include <ppl.h>


namespace raycore {
namespace tracer {

Renderer::Renderer(unsigned int width, unsigned int height, const Settings &settings) :
	width(width), 
	height(height), 
	camera(nullptr), 
	tracer(nullptr),
	tileSize(settings.tileSize),
	samples(0),
	m_output(width * height),
	subSamplesX(settings.samplesX),
	subSamplesY(settings.samplesY),
	raySamplerX(settings.raySamplerX),
	raySamplerY(settings.raySamplerY)
{
	this->buildTiles(tileSize);
}

Renderer::~Renderer()
{
	if (this->camera != nullptr)
		delete this->camera;
	if (this->tracer != nullptr)
		delete this->tracer;
}

void Renderer::setScene(Scene &&scene)
{
	this->scene = scene;
	this->scene.build();
}

bool Renderer::updateRays()
{
	if (!this->camera->computeTransform())
		return false;
	this->tracer->reset();
	samples = 0;
	return true;
}

bool Renderer::renderPreview()
{
	Log::info("Rendering preview");

	for (auto it = tiles.begin(); it != tiles.end(); it++)
	{
		Tile &tile = (*it);
		index2D center = tile.min + tile.center();
		Ray ray = this->camera->generateRay(
			RayIndex(center.x, this->width, RaySampler::LINEAR),
			RayIndex(center.y, this->height, RaySampler::LINEAR)
		);
		color4f p = this->tracer->castRay(ray, this->scene, 5);
		for (unsigned int y = tile.min.y; y < tile.max.y; y++)
			for (unsigned int x = tile.min.x; x < tile.max.x; x++)
				m_output[y * this->width + x] = p;
	}
	return true;
}

bool Renderer::render()
{
	Log::info("Rendering sample ", (samples + 1));
#if defined(PARALLEL_RENDERING)
	concurrency::parallel_for(size_t(0), tiles.size(), [&](size_t iTile)
	{
		Tile &tile = this->tiles[iTile];
		const float c = (1.f / (this->subSamplesX * this->subSamplesY));
		for (unsigned int y = tile.min.y; y < tile.max.y; y++)
		{
			for (unsigned int x = tile.min.x; x < tile.max.x; x++)
			{
				color4f p(0.f);
				for (unsigned int sy = 0; sy < this->subSamplesY; sy++)
				{
					for (unsigned int sx = 0; sx < this->subSamplesX; sx++)
					{
						Ray ray = this->camera->generateRay(
							RayIndex(x * this->subSamplesX + sx, this->width * this->subSamplesX, this->raySamplerX),
							RayIndex(y * this->subSamplesY + sy, this->height * this->subSamplesY, this->raySamplerY)
						);
						p += this->tracer->castRay(ray, this->scene, Config::maxDepth) * c;
					}
				}
				color4f &output = m_output[y * this->width + x];
				m_output[y * this->width + x] = geometry::lerp(output, p, 1.f / (samples + 1.f));
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


void Renderer::resize(unsigned int width, unsigned int height)
{
	throw std::runtime_error("Not implemented");
}

void Renderer::buildTiles(unsigned int tileSize) {
	index2D nTiles(
		static_cast<int>(std::ceil(this->width / static_cast<float>(tileSize))),
		static_cast<int>(std::ceil(this->height / static_cast<float>(tileSize)))
	);
	tiles.clear();
	for (unsigned int y = 0; y < nTiles.y; y++)
	{
		for (unsigned int x = 0; x < nTiles.x; x++)
		{
			this->tiles.push_back(Tile(
				index2D(x * tileSize, y * tileSize),
				index2D(geometry::min((x + 1) * tileSize, this->width), geometry::min((y + 1) * tileSize, this->height))
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
const std::vector<color4f> & Renderer::image() const
{
	return m_output;
}
}
}