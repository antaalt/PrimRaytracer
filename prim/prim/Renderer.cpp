#include "Renderer.h"
#include "Config.h"
#include "Ray.h"
#include "Tracer.h"
#include "Camera.h"


namespace prim {

Renderer::Renderer(Tracer &tracer, uint32_t width, uint32_t height) :
	m_samples(0),
	m_tracer(tracer),
	m_tiles(),
	m_output(width * height, color4f(0.f)),
	m_threadPool(),
	m_width(width), 
	m_height(height)
{
	generateTiles();
	m_threadPool.start();
}

Renderer::~Renderer()
{
	m_threadPool.stop();
}

void Renderer::resize(uint32_t width, uint32_t height)
{
	m_threadPool.stop();
	m_threadPool.start();
	std::lock_guard<std::mutex> lock(m_mutexOutput);
	m_output.resize(width * height, color4f(0.f));
	m_width = width;
	m_height = height;
	generateTiles();
}

void Renderer::reset()
{
	m_samples = 0;
	m_threadPool.reset();
}

bool Renderer::isWaiting() const
{
	return m_threadPool.empty();
}

void Renderer::launch(const Camera & camera, const Scene & scene)
{
	static uint32_t samples = 0;
	samples = m_samples;
	m_completedTiles = 0;
	for (Tile &tile : m_tiles)
	{
		m_threadPool.addTask([&]() {
			for (uint32_t y = tile.offset.y; y < tile.offset.y + tile.size.y; y++)
			{
				for (uint32_t x = tile.offset.x; x < tile.offset.x + tile.size.x; x++)
				{
					RaySampler::Type sample = m_sampler(vec2u(x, y), vec2u(m_width, m_height));
					Ray ray = camera.generateRay(sample);
					color4f p = color4f::linear2srgb(m_tracer.render(ray, scene));
					std::lock_guard<std::mutex> lock(m_mutexOutput);
					color4f &output = m_output[y * m_width + x];
					m_output[y * m_width + x] = geometry::lerp(output, p, 1.f / (samples + 1.f));
				}
			}
			m_completedTiles++;
		});
	}
	//m_threadPool.wait();
	m_samples++;
}

void Renderer::getOutput(color4f * data, size_t offset, size_t size)
{
	std::lock_guard<std::mutex> lock(m_mutexOutput);
	memcpy(data, m_output.data() + offset, size * sizeof(color4f));
}

void Renderer::generateTiles()
{
	m_tiles.clear();
	const vec2u tileSize(32, 32);
	const vec2u imageSize(m_width, m_height);
	const vec2u tileCount(vec2f(imageSize) / vec2f(tileSize));
	const vec2u tileRemainderSize(imageSize - tileCount * tileSize);
	for (uint32_t y = 0; y < tileCount.y; y++)
	{
		for (uint32_t x = 0; x < tileCount.x; x++)
			m_tiles.push_back(Tile{ vec2u(tileSize.x * x, tileSize.y * y), tileSize });
		if (tileRemainderSize.x > 0)
			m_tiles.push_back(Tile{ vec2u(tileSize.x * tileCount.x, tileSize.y * y), vec2u(tileRemainderSize.x, tileSize.y) });
	}
	if (tileRemainderSize.y > 0)
		for (uint32_t x = 0; x < tileCount.x; x++)
			m_tiles.push_back(Tile{ vec2u(tileSize.x * x, tileSize.y * tileCount.y), vec2u(tileSize.x, tileRemainderSize.y) });
}

}