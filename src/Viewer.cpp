#include "Viewer.h"

#include <Aka/Aka.h>

#include "Prim/ThreadPool.h"
#include "Prim/PathTracer.h"
#include "Prim/OBJLoader.h"
#include "Prim/PerspectiveCamera.h"

namespace viewer {
using namespace aka;

static prim::Scene scene;
static prim::PerspectiveCamera camera;
static prim::PathTracer tracer(6);
static prim::ThreadPool threadPool;
static Time::Unit renderTime = Time::now();
static Texture::Ptr texture;
static Batch batch;
static uint32_t samples = 0;
static uint32_t swidth;
static uint32_t sheight;


struct Tile {
	vec2u offset;
	vec2u size;
	std::vector<color4f> output;
};
static std::vector<const Tile*> tilesFinished;
static std::vector<Tile> tiles;

static std::mutex mutex;

std::vector<Tile> generateTiles(uint32_t width, uint32_t height)
{
	std::vector<Tile> tiles;
	const vec2u tileSize(32, 32);
	const vec2u imageSize(width, height);
	const vec2u tileCount(vec2f(imageSize) / vec2f(tileSize));
	const vec2u tileRemainderSize(imageSize - tileCount * tileSize);
	for (uint32_t y = 0; y < tileCount.y; y++)
	{
		for (uint32_t x = 0; x < tileCount.x; x++)
			tiles.push_back(Tile{ 
				vec2u(tileSize.x * x, tileSize.y * y), 
				tileSize, 
				std::vector<color4f>(tileSize.x * tileSize.y, color4f(0.f, 0.f, 0.f, 1.f))
			});
		if (tileRemainderSize.x > 0)
			tiles.push_back(Tile{ 
				vec2u(tileSize.x * tileCount.x, tileSize.y * y), 
				vec2u(tileRemainderSize.x, tileSize.y), 
				std::vector<color4f>(tileSize.x * tileSize.y, color4f(0.f, 0.f, 0.f, 1.f))
			});
	}
	if (tileRemainderSize.y > 0)
		for (uint32_t x = 0; x < tileCount.x; x++)
			tiles.push_back(Tile{
				vec2u(tileSize.x * x, tileSize.y * tileCount.y),
				vec2u(tileSize.x, tileRemainderSize.y), 
				std::vector<color4f>(tileSize.x * tileSize.y, color4f(0.f, 0.f, 0.f, 1.f))
			});
	return tiles;
}

void launch()
{
	Logger::info("Launching render (previous : ", (Time::now() - renderTime).milliseconds(), "ms) - samples : ", samples);
	renderTime = Time::now();
	for (Tile& tile : tiles)
	{
		threadPool.addTask([&]() {
			prim::RandomRaySampler sampler;
			for (uint32_t y = 0; y < tile.size.y; y++)
			{
				for (uint32_t x = 0; x < tile.size.x; x++)
				{
					prim::RaySampler::Type sample = sampler(vec2u(tile.offset.x + x, tile.offset.y + y), vec2u(swidth, sheight));
					prim::Ray ray = camera.generateRay(sample);
					color4f outSrgb = color4f::linear2srgb(tracer.render(ray, scene));
					tile.output[y * tile.size.x + x] = geometry::lerp(tile.output[y * tile.size.x + x], outSrgb, 1.f / (samples + 1.f));
					tile.output[y * tile.size.x + x].a = 1.f;
				}
			}
			// Notify tile finished. use event ?
			std::lock_guard<std::mutex> m(mutex);
			tilesFinished.push_back(&tile);
		});
	}
}

void Viewer::initialize()
{
	tiles = generateTiles(width(), height());
	threadPool.start();
	{
		// Set scene
		using namespace prim;
#if 0
		scene.textures.push_back(new ConstantTexture4f(color4f(0.9f)));
		scene.textures.push_back(new ConstantTexture4f(color4f(0.5f, 1.f, 0.5f, 1.f)));
		scene.textures.push_back(new ConstantTexture4f(color4f(0.82f, 0.62f, 0.19f, 1.f)));
		scene.textures.push_back(new CheckerTexture4f(color4f(0.1f), color4f(1.f), 40, 40));
		scene.textures.push_back(new CheckerTexture4f(color4f(0.5f, 0.5f, 1.f, 1.f), color4f(1.f, 0.5f, 0.5f, 1.f), 2, 2));
		ConstantTextureFloat* roughness = new ConstantTextureFloat(0.5f); // TODO fix leak

		scene.materials.push_back(new Glass(scene.textures[0], 1.1f));
		scene.materials.push_back(new Matte(scene.textures[1]));
		scene.materials.push_back(new Metal(scene.textures[2], roughness));
		scene.materials.push_back(new Matte(scene.textures[3]));
		scene.materials.push_back(new Matte(scene.textures[4]));

		scene.nodes.push_back(new TransformNode(mat4f::translate(vec3f(0.f, 0.f, 1.f)), new Sphere(0.5f, scene.materials[0])));
		scene.nodes.push_back(new TransformNode(mat4f::translate(vec3f(0.f, 0.1f, -1.0f)), new Sphere(0.5f, scene.materials[1])));
		scene.nodes.push_back(new TransformNode(mat4f::translate(vec3f(-1.5f, 0.2f, 0.f)), new Sphere(0.6f, scene.materials[2])));
		scene.nodes.push_back(new TransformNode(mat4f::translate(vec3f(0.f, -30.f, 0.f)), new Sphere(29.f, scene.materials[3])));
		scene.nodes.push_back(new TransformNode(mat4f::translate(vec3f(1.5f, 0.2f, 0.f)), new Sphere(0.6f, scene.materials[4])));
#if 0
		{
			OBJLoader loader;
			loader.load("models/bunny/bunny.obj", scene);
			scene.nodes.back()->setTransform(Transform(mat4f::translate(vec3f(0.f, 1.f, 0.f)) * mat4f::scale(vec3f(15.f))));
		}
#endif
#else
		{
			OBJLoader loader; 
			Path path = Asset::path("models/Sponza/sponza.obj");
			loader.load(path, scene);
			scene.nodes.back()->setTransform(Transform(mat4f::scale(vec3f(0.1f))));
			//scene.nodes.back()->setTransform(Transform(mat4f::scale(vec3f(15.f))));
		}
#endif
		scene.lights.push_back(new SunLight(norm3f(0.f, 1.f, 0.f), color4f(1.f), 19000.f));
	}
	{
		camera.perspective = mat4f::perspective(degreef(60.f), width() / (float)height(), 0.1f, 1000.f);
		camera.hFov = 60.f;
		camera.transform = prim::Transform(mat4f::translate(vec3f(
			0.f,
			1.f,
			0.f
		)) * mat4f::rotate(vec3f(0.f, 1.f, 0.f), degreef(90.f)));
	}
	{
		Sampler sampler;
		sampler.filterMag = Sampler::Filter::Nearest;
		sampler.filterMin = Sampler::Filter::Nearest;
		sampler.wrapS = Sampler::Wrap::Clamp;
		sampler.wrapT = Sampler::Wrap::Clamp;
		texture = Texture::create(width(), height(), Texture::Format::Float, Texture::Component::RGBA, sampler);
		std::vector<color4f> bytes(swidth * sheight, color4f(0.f, 0.f, 0.f, 1.f));
		texture->upload(bytes.data());
	}
	swidth = width();
	sheight = height();

	scene.build();

	Framebuffer::Ptr backbuffer = GraphicBackend::backbuffer();
	backbuffer->clear(0.f, 0.f, 0.f, 1.f);

	launch();
}

void Viewer::destroy()
{
	threadPool.stop();
	GraphicBackend::screenshot("output.png");
}

void Viewer::update(aka::Time::Unit deltaTime)
{
	bool updated = false;
	mat4f transform = camera.transform.getMatrix();
	if (input::pressed(input::Button::Button1))
	{
		updated = true;
		const input::Position& delta = input::delta();
		transform *= mat4f::rotate(vec3f(0.f, 1.f, 0.f), degreef(static_cast<float>(-delta.x)));
		transform *= mat4f::rotate(vec3f(1.f, 0.f, 0.f), degreef(static_cast<float>(-delta.y)));
	}
	if (input::pressed(input::Button::Button2))
	{
		updated = true;
		const input::Position& delta = input::delta();
		transform *= geometry::mat4f::translate(vec3f(-delta.x * 0.01f, delta.y * 0.01f, 0.f));
	}
	const input::Position& scroll = input::scroll();
	if (scroll.y != 0.f)
	{
		updated = true;
		transform *= geometry::mat4f::translate(vec3f(0.f, 0.f, scroll.y * 0.1f));
	}
	camera.transform = prim::Transform(transform);
	if (updated)
	{
		threadPool.reset();
		samples = 0;
		launch();
	}
	if (aka::input::pressed(aka::input::Key::Escape))
		quit();
}

void Viewer::render()
{
	Time::Unit start = Time::now();

	if (tilesFinished.size() > 0)
	{
		std::lock_guard<std::mutex> m(mutex);
		for (const Tile* tile : tilesFinished)
		{
			Rect region;
			region.x = tile->offset.x;
			region.y = tile->offset.y;
			region.w = tile->size.x;
			region.h = tile->size.y;
			texture->upload(region, tile->output.data());
		}
		tilesFinished.clear();
	}
	if (threadPool.empty())
	{
		samples++;
		launch();
	}

	batch.draw(mat3f::identity(), Batch::Rect(vec2f(0), vec2f((float)width(), (float)height()), texture, 0));
	std::unique_lock<std::mutex> m(mutex);
	batch.render(GraphicBackend::backbuffer(), mat4f::identity(), mat4f::orthographic(0.f, (float)height(), 0.f, (float)width()));
	m.unlock();
	batch.clear();
}

void Viewer::resize(uint32_t width, uint32_t height) 
{
	{
		threadPool.reset();
		swidth = width;
		sheight = height;
		samples = 0;
		tiles = generateTiles(width, height);
	}
	{
		Sampler sampler;
		sampler.filterMag = Sampler::Filter::Nearest;
		sampler.filterMin = Sampler::Filter::Nearest;
		sampler.wrapS = Sampler::Wrap::Clamp;
		sampler.wrapT = Sampler::Wrap::Clamp;
		texture = Texture::create(width, height, Texture::Format::Float, Texture::Component::RGBA, sampler);
		std::vector<color4f> bytes(swidth * sheight, color4f(0.f, 0.f, 0.f, 1.f));
		texture->upload(bytes.data());
	}
	launch();
}


};