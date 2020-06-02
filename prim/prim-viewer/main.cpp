#include "Application.h"
#include "WhittedTracer.h"
#include "PathTracer.h"
#include "PerspectiveCamera.h"
#include "Loader.h"
#include "Sphere.h"
#include "OBJLoader.h"

#include "math/geometry.h"

#include <fstream>

void setScene(prim::Scene &scene)
{
	using namespace prim;
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
#if 0
	{
		OBJLoader loader;
		std::vector<uint8_t> data;
		{
			std::ifstream file("../prim/data/models/bunny/bunny.obj", std::ios::ate);
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
#endif
}

int main(int argc, char *argv[])
{
	unsigned int width = 800;
	unsigned int height = 600;
	
	// Set parameters
	prim::PathTracer tracer(6);
	prim::Scene scene;
	prim::PerspectiveCamera camera;
	camera.perspective = geometry::mat4f::perspective(geometry::degreef(60.f), width / (float)height, 0.1f, 1000.f);
	camera.hFov = 60.f;
	camera.transform = geometry::mat4f::lookAt(geometry::point3f(
		0.f,
		4.f,
		4.f
	), geometry::point3f(0.f, 0.f, 0.f));

	// Load Scene
	Log::info("Loading scene");
	setScene(scene);

	// Launch app
	app::Application application(width, height);
	application.run(scene, camera, tracer);

	return 1;
}