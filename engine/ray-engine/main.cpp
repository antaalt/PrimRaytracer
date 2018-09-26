#include "Application.h"
#include "WhittedTracer.h"
#include "PinholeCamera.h"
#include "SceneBuilder.h"
#include "stb_image_write.h"

int main(int argc, char *argv[])
{
	unsigned int width = 480;
	unsigned int height = 360;
	app::Application application(width, height);
	// Set options
	app::options options;
	options.tracer = new raycore::tracer::WhittedTracer();
	options.camera = new raycore::tracer::PinholeCamera(width, height);
	options.camera->lookAt(point3(
		0.f,
		0.f,
		1.f
	), point3(0.f, 0.f, 0.f));
	options.acceleration = raycore::tracer::Acceleration::BVH;

	// Load Scene
	Log::info("Loading scene");
	raycore::Scene scene;
	//app::SceneBuilder::buildCustomScene(scene);
	//app::Scene::GLTF::load("../data/models/lantern/Lantern.gltf", scene);
	//app::Scene::GLTF::load("../data/models/duck/Duck.gltf", scene);
	//app::Scene::GLTF::load("../data/models/box/box.gltf", scene);
	//app::Scene::GLTF::load("../data/models/boxTextured/BoxTextured.gltf", scene);
	raycore::Scene::GLTF::load("../data/models/sponza/Sponza.gltf", scene);
	raycore::Light l;
	l.position = vec3(2.f, 2.f, 2.f);
	scene.lights.push_back(l);
	application.run(scene, options);

	system("pause");
	return 1;
}