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
#if 0
	scene.textures.push_back(new ConstantTexture4f(color4f(0.9f)));
	scene.textures.push_back(new ConstantTexture4f(color4f(0.5f, 1.f, 0.5f, 1.f)));
	scene.textures.push_back(new ConstantTexture4f(color4f(0.82f, 0.62f, 0.19f, 1.f)));
	scene.textures.push_back(new CheckerTexture4f(color4f(0.1f), color4f(1.f), 40, 40));
	scene.textures.push_back(new CheckerTexture4f(color4f(0.5f, 0.5f, 1.f, 1.f), color4f(1.f, 0.5f, 0.5f, 1.f), 2, 2));
	ConstantTextureFloat *roughness = new ConstantTextureFloat(0.5f); // TODO fix leak
	
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
		loader.load("./data/models/bunny/bunny.obj", scene);
		scene.nodes.back()->setTransform(Transform(mat4f::translate(vec3f(0.f, 1.f, 0.f)) * mat4f::scale(vec3f(15.f))));
	}
#endif
#else
	{
		OBJLoader loader;
		loader.load("./data/models/sponza/sponza.obj", scene);
		scene.nodes.back()->setTransform(Transform(mat4f::scale(vec3f(0.1f))));
		//scene.nodes.back()->setTransform(Transform(mat4f::scale(vec3f(15.f))));
	}
#endif
	scene.lights.push_back(new SunLight(norm3f(0.f, 1.f, 0.f), color4f(1.f), 19000.f));
}

int main(int argc, char *argv[])
{
	unsigned int width = 800;
	unsigned int height = 600;
	
	// Set parameters
	prim::PathTracer tracer(6);
	prim::Scene scene;
	prim::PerspectiveCamera camera;
	camera.perspective = mat4f::perspective(degreef(60.f), width / (float)height, 0.1f, 1000.f);
	camera.hFov = 60.f;
	camera.transform = prim::Transform(mat4f::translate(vec3f(
		0.f,
		10.f,
		0.f
	)) * mat4f::rotate(vec3f(0.f, 1.f, 0.f), degreef(90.f)));

	// Load Scene
	Log::info("Loading scene");
	setScene(scene);

	// Launch app
	app::Application application(width, height);
	application.run(scene, camera, tracer);

	return 1;
}