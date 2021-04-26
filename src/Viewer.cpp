#include "Viewer.h"

#include <Aka/Aka.h>

#include "Editor/SceneUI.h"

#include "Prim/ThreadPool.h"
#include "Prim/Tracer/PathTracer.h"
#include "Prim/Scene/Loader/OBJLoader.h"
#include "Prim/Scene/Camera/PerspectiveCamera.h"
#include "Prim/Scene/Light/SunLight.h"

#include "Prim/Scene/Texture/ConstantTexture.h"
#include "Prim/Scene/Texture/ImageTexture.h"
#include "Prim/Scene/Texture/CheckerTexture.h"

#include "Prim/Scene/Hitable/Sphere.h"
#include "Prim/Scene/Hitable/TransformNode.h"

#include "Prim/Scene/Material/Glass.h"
#include "Prim/Scene/Material/Matte.h"
#include "Prim/Scene/Material/Metal.h"

namespace viewer {
using namespace aka;

static prim::Scene scene;
static prim::PerspectiveCamera camera;
static prim::PathTracer tracer(6);
static prim::ThreadPool threadPool;
static Time::Unit renderTime = Time::now();
static Texture::Ptr texture;
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
static std::deque<Rect> renderTile;

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
				std::vector<color4f>(tileRemainderSize.x * tileSize.y, color4f(0.f, 0.f, 0.f, 1.f))
			});
	}
	if (tileRemainderSize.y > 0)
	{
		for (uint32_t x = 0; x < tileCount.x; x++)
			tiles.push_back(Tile{
				vec2u(tileSize.x * x, tileSize.y * tileCount.y),
				vec2u(tileSize.x, tileRemainderSize.y),
				std::vector<color4f>(tileSize.x * tileRemainderSize.y, color4f(0.f, 0.f, 0.f, 1.f))
			});
		if (tileRemainderSize.x > 0)
			tiles.push_back(Tile{
				vec2u(tileSize.x * tileCount.x, tileSize.y * tileCount.y),
				vec2u(tileRemainderSize.x, tileRemainderSize.y),
				std::vector<color4f>(tileRemainderSize.x * tileRemainderSize.y, color4f(0.f, 0.f, 0.f, 1.f))
			});
	}
	return tiles;
}

void launch()
{
	renderTime = Time::now();
	for (Tile& tile : tiles)
	{
		threadPool.addTask([&]() {
			prim::RandomRaySampler sampler{ samples };
			for (uint32_t y = 0; y < tile.size.y; y++)
			{
				for (uint32_t x = 0; x < tile.size.x; x++)
				{
					prim::RaySampler::Type sample = sampler(vec2u(tile.offset.x + x, tile.offset.y + y), vec2u(swidth, sheight));
					prim::Ray ray = camera.generateRay(sample);
					color4f outSrgb = color4f::linear2srgb(tracer.render(ray, scene));
					tile.output[y * tile.size.x + x] = lerp(tile.output[y * tile.size.x + x], outSrgb, 1.f / (samples + 1.f));
					tile.output[y * tile.size.x + x].a = 1.f;
				}
			}
			// Notify tile finished. use event ?
			std::lock_guard<std::mutex> m(mutex);
			tilesFinished.push_back(&tile);
		});
	}
}

void Viewer::onCreate()
{
	Framebuffer::Ptr backbuffer = GraphicBackend::backbuffer();
	tiles = generateTiles(backbuffer->width(), backbuffer->height());
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
#if 1
		{
			OBJLoader loader;
			Path path = Asset::path("models/Bunny/bunny.obj");
			loader.load(path, scene);
			reinterpret_cast<TransformNode*>(scene.nodes.back())->setTransform(Transform(
				mat4f::translate(vec3f(0.f, 1.f, 0.f)) * mat4f::scale(vec3f(15.f))
			));
		}
#endif
#else
		{
			OBJLoader loader; 
			Path path = Asset::path("models/Sponza/sponza.obj");
			loader.load(path, scene);
		}
#endif
		scene.lights.push_back(new SunLight(norm3f(0.f, 1.f, 0.f), color4f(1.f), 19000.f));
	}
	{
		camera.perspective = mat4f::perspective(anglef::degree(60.f), backbuffer->width() / (float)backbuffer->height(), 0.1f, 1000.f);
		camera.inverse = mat4f::inverse(camera.perspective);
		camera.hFov = 60.f;
		camera.transform = prim::Transform(mat4f::translate(vec3f(
			0.f,
			1.f,
			0.f
		)) * mat4f::rotate(vec3f(0.f, 1.f, 0.f), anglef::degree(90.f)));
	}
	{
		Sampler sampler;
		sampler.filterMag = Sampler::Filter::Nearest;
		sampler.filterMin = Sampler::Filter::Nearest;
		sampler.wrapU = Sampler::Wrap::ClampToEdge;
		sampler.wrapV = Sampler::Wrap::ClampToEdge;
		texture = Texture::create(backbuffer->width(), backbuffer->height(), TextureFormat::Float, TextureComponent::RGBA, TextureFlag::None, sampler);
		std::vector<color4f> bytes(swidth * sheight, color4f(0.f, 0.f, 0.f, 1.f));
		texture->upload(bytes.data());
	}
	swidth = backbuffer->width();
	sheight = backbuffer->height();

	scene.build();

	m_sceneUI = m_editor.attach<SceneUI>();
	m_sceneUI->setScene(&scene);
	m_sceneUI->setCamera(&camera);
	m_editor.initialize();

	backbuffer->clear(color4f(0.f, 0.f, 0.f, 1.f));

	launch();
}

void Viewer::onDestroy()
{
	m_editor.destroy();
	threadPool.stop();
	ImageHDR hdr(texture->width(), texture->height(), 4);
	texture->download(hdr.bytes.data());
	Image img(hdr);
	img.save("output.png");
}

void Viewer::onFrame()
{
	m_editor.frame();
}

void Viewer::onUpdate(aka::Time::Unit deltaTime)
{
	bool updated = false;
	mat4f transform = camera.transform.getMatrix();
	if (!m_editor.focused() && Mouse::pressed(MouseButton::Button1))
	{
		const Position& delta = Mouse::delta();
		if (delta.x != 0.f || delta.y != 0.f)
		{
			transform *= mat4f::rotate(vec3f(0.f, 1.f, 0.f), anglef::degree(delta.x * deltaTime.seconds() * 10.f));
			transform *= mat4f::rotate(vec3f(1.f, 0.f, 0.f), anglef::degree(delta.y * deltaTime.seconds() * 10.f));
			updated = true;
		}
	}
	if (!m_editor.focused() && Mouse::pressed(MouseButton::Button2))
	{
		const Position& delta = Mouse::delta();
		if (delta.x != 0.f || delta.y != 0.f)
		{
			transform *= geometry::mat4f::translate(vec3f(delta.x * deltaTime.seconds() * 0.5f, -delta.y * deltaTime.seconds() * 0.5f, 0.f));
			updated = true;
		}
	}
	const Position& scroll = Mouse::scroll();
	if (!m_editor.focused() && scroll.y != 0.f)
	{
		if (scroll.y != 0.f)
		{
			updated = true;
			transform *= geometry::mat4f::translate(vec3f(0.f, 0.f, scroll.y * deltaTime.seconds() * 10.f));
		}
	}
	camera.transform = prim::Transform(transform);
	if (updated)
	{
		threadPool.reset();
		samples = 0;
		launch();
	}
	if (Keyboard::pressed(KeyboardKey::Escape))
		EventDispatcher<QuitEvent>::emit();
	m_editor.update(deltaTime);
}

void Viewer::onRender()
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
			// Store tile to draw them
			renderTile.push_back(region);
			if (renderTile.size() > threadPool.size())
				renderTile.pop_front();
		}
		tilesFinished.clear();
	}
	if (threadPool.empty())
	{
		samples++;
		m_sceneUI->setSampleCount(samples);
		launch();
	}

	Framebuffer::Ptr backbuffer = GraphicBackend::backbuffer();
	backbuffer->clear(color4f(0.f, 0.f, 0.f, 1.f));
	Renderer2D::drawRect(mat3f::identity(), vec2f(0), vec2f((float)backbuffer->width(), (float)backbuffer->height()), texture, color4f(1.f), 0);
	for (const Rect& rect : renderTile)
		Renderer2D::drawRect(mat3f::identity(), vec2f(rect.x, rect.y), vec2f(rect.w, rect.h), nullptr, color4f(0.93f, 0.04f, 0.26f, 0.2f), 1);

	Renderer2D::render(backbuffer, mat4f::identity(), mat4f::orthographic(0.f, (float)backbuffer->height(), 0.f, (float)backbuffer->width()));
	Renderer2D::clear();

	m_editor.draw();
	m_editor.render();
}

void Viewer::onResize(uint32_t width, uint32_t height) 
{
	{
		// Wait for thread pool
		threadPool.stop();
		threadPool.start();
	}
	{
		swidth = width;
		sheight = height;
		samples = 0;
		tilesFinished.clear();
		tiles = generateTiles(width, height);
		camera.perspective = mat4f::perspective(anglef::degree(camera.hFov), width / (float)height, 0.1f, 1000.f);
		camera.inverse = mat4f::inverse(camera.perspective);
	}
	{
		Sampler sampler;
		sampler.filterMag = Sampler::Filter::Nearest;
		sampler.filterMin = Sampler::Filter::Nearest;
		sampler.wrapU = Sampler::Wrap::ClampToEdge;
		sampler.wrapV = Sampler::Wrap::ClampToEdge;
		texture = Texture::create(width, height, TextureFormat::Float, TextureComponent::RGBA, TextureFlag::None, sampler);
		std::vector<color4f> bytes(swidth * sheight, color4f(0.f, 0.f, 0.f, 1.f));
		texture->upload(bytes.data());
	}
	launch();
}


};