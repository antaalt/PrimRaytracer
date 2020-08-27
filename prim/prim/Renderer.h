#pragma once

#include "Config.h"
#include "Tracer.h"
#include "Camera.h"
#include "ThreadPool.h"

#include <string>
#include <atomic>

namespace prim {

struct Tile {
	vec2u offset;
	vec2u size;
};

struct Renderer2 {

	// setup tiles & co
	void build(uint32_t width, uint32_t height);

	void render();

	std::function<void()> onTileComplete;
	std::function<void()> onRenderFinished;

	std::vector<Tile> tiles;
};

class Renderer {
public:
	Renderer(Tracer &tracer, uint32_t width, uint32_t height);
	~Renderer();

	void resize(uint32_t width, uint32_t height);
	void reset();
	bool isWaiting() const;
	void launch(const Camera &camera, const Scene &scene);
	void getOutput(color4f *data, size_t offset, size_t size);
	size_t getTileCount() const { return m_tiles.size(); }
	size_t getCompletedTileCount() const { return m_completedTiles; }
	uint32_t getSampleCount() const { return m_samples; }
private:
	void generateTiles();
private:
	RandomRaySampler m_sampler;
	uint32_t m_samples;
	Tracer &m_tracer;
	std::vector<Tile> m_tiles;
	std::atomic<size_t> m_completedTiles;
	std::mutex m_mutexOutput;
	std::vector<color4f> m_output;
	ThreadPool m_threadPool;
	uint32_t m_width, m_height;
};

}