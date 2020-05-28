#pragma once

#include "Config.h"
#include "Tracer.h"
#include "Camera.h"
#include "PixelBuffer.h"

#include <string>

#define TILE_WIDTH_NUMBER 20
#define TILE_HEIGHT_NUMBER 20
#define PARALLEL_RENDERING

namespace raycore {
namespace tracer {

using index2D = vec2<unsigned int>;

struct Tile {
	Tile(const index2D &min, const index2D &max): min(min), max(max) {}

	unsigned int width() { return max.x - min.x; }
	unsigned int height() { return max.y - min.y; }
	index2D center() { index2D index(max - min); index.x *= 0.5f; index.y *= 0.5f; return index; }

	index2D min;
	index2D max;
};

struct Settings {
	unsigned int tileSize;
	unsigned int samplesX;
	unsigned int samplesY;
	RaySampler raySamplerX;
	RaySampler raySamplerY;
};

class Renderer
{
public:
	Renderer(unsigned int width, unsigned int height, const Settings &settings);
	~Renderer();
	Renderer(const Renderer& other) = delete;
	Renderer& operator=(const Renderer &other) = delete;

	bool buildScene(prim::Scene &&scene, prim::Acceleration acceleration);

	bool updateRays();

	bool renderPreview();

	bool render();

	void resize(unsigned int width, unsigned int height);

	void buildTiles(unsigned int tileSize);

	void setTracer(tracer::Tracer* tracer);
	void setCamera(tracer::Camera* camera);

	const std::vector<color4f> &image() const;

private:
	Camera* camera;
	Tracer* tracer; // TODO add a secondary quick tracer (whitted for example) for display
	prim::Scene scene;

	std::vector<Tile> tiles;
	unsigned int tileSize;
	unsigned int width, height;
	unsigned int samples;
	unsigned int subSamplesX, subSamplesY;
	RaySampler raySamplerX, raySamplerY;
	std::vector<color4f> m_output;
};

}
}