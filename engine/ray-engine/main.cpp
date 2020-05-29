#include "Application.h"
#include "WhittedTracer.h"
#include "PathTracer.h"
#include "PinholeCamera.h"
#include "Loader.h"
#include "Sphere.h"
#include "OBJLoader.h"

#include "math/geometry.h"

#include <fstream>

void setScene(raycore::Scene &scene)
{
	using namespace raycore;
	using namespace geometry;

	scene.textures.push_back(new ConstantTexture<float>(color4f(0.9f)));
	scene.textures.push_back(new ConstantTexture<float>(color4f(0.5f, 1.f, 0.5f, 1.f)));
	scene.textures.push_back(new ConstantTexture<float>(color4f(0.82f, 0.62f, 0.19f, 1.f)));
	scene.textures.push_back(new Checker<float>(color4f(0.1f), color4f(1.f), 40, 40));
	scene.textures.push_back(new Checker<float>(color4f(0.5f, 0.5f, 1.f, 1.f), color4f(1.f, 0.5f, 0.5f, 1.f), 2, 2));
	scene.textures.push_back(new ConstantTexture<float>(color4f(0.1f)));
	
	scene.materials.push_back(new Glass(scene.textures[0], 1.1f));
	scene.materials.push_back(new Matte(scene.textures[1]));
	scene.materials.push_back(new Metal(scene.textures[2], scene.textures[5]));
	scene.materials.push_back(new Matte(scene.textures[3]));
	scene.materials.push_back(new Matte(scene.textures[4]));

	scene.hitables.push_back(new Sphere(mat4f::translate(vec3f(0.f)), 0.4f, scene.materials[0]));
	scene.hitables.push_back(new Sphere(mat4f::translate(vec3f(0.f, -1.0f, 0.1f)), 0.5f, scene.materials[1]));
	scene.hitables.push_back(new Sphere(mat4f::translate(vec3f(-1.5f, 0.f, 0.2f)), 0.6f, scene.materials[2]));
	scene.hitables.push_back(new Sphere(mat4f::translate(vec3f(0.f, 0.f, -30.f)), 29.f, scene.materials[3]));
	scene.hitables.push_back(new Sphere(mat4f::translate(vec3f(-1.5f, 0.f, 0.2f)), 0.6f, scene.materials[4]));

	{
		OBJLoader loader;
		std::vector<uint8_t> data;
		{
			std::ifstream file("../raycore/data/models/bunny/bunny.obj", std::ios::ate);
			if (!file)
				throw std::runtime_error("Could not load file");
			std::streampos size = file.tellg();
			file.seekg(0);
			data.resize(size);
			file.read(reinterpret_cast<char*>(data.data()), size);
		}
		MemoryReader reader(data);
		loader.load(reader, scene);
		scene.hitables.back()->setTransform(geometry::mat4f::scale(vec3f(15.f)));
	}
}

int main(int argc, char *argv[])
{
	unsigned int width = 800;
	unsigned int height = 600;
	app::Application application(width, height);
	// Set options
	app::options options;
	options.tracer = new raycore::tracer::PathTracer();
	options.camera = new raycore::tracer::PinholeCamera(0.f, 0.f);
	/*options.camera->lookAt(point3f(
		1.f,
		1.f,
		0.f
	), point3f(0.f, 2.f, 0.f));*/
	options.camera->lookAt(geometry::point3f(
		0.f,
		4.f,
		4.f
	), geometry::point3f(0.f, 0.f, 0.f));
	options.settings.raySamplerX = raycore::tracer::RaySampler::RANDOM;
	options.settings.raySamplerY = raycore::tracer::RaySampler::RANDOM;
	options.settings.samplesX = 1;
	options.settings.samplesY = 1;
	options.settings.tileSize = 32;

	// Load Scene
	Log::info("Loading scene");
	raycore::Scene scene;
	setScene(scene);
	application.run(scene, options);

	system("pause");
	return 1;
}