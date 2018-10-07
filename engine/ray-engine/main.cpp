#include "Application.h"
#include "WhittedTracer.h"
#include "PathTracer.h"
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
	options.camera = new raycore::tracer::PinholeCamera();
	/*options.camera->lookAt(point3(
		1.f,
		1.f,
		0.f
	), point3(0.f, 2.f, 0.f));*/
	options.camera->lookAt(point3(
		0.8f,
		0.f,
		0.f
	), point3(0.f, 0.f, 0.f));
	options.acceleration = raycore::tracer::Acceleration::BVH;
	options.settings.raySamplerX = raycore::tracer::RaySampler::LINEAR;
	options.settings.raySamplerY = raycore::tracer::RaySampler::LINEAR;
	options.settings.samplesX = 1;
	options.settings.samplesY = 1;
	options.settings.tileSize = 32;

	// Load Scene
	Log::info("Loading scene");
	raycore::Scene scene;
	//raycore::SceneBuilder::buildCustomScene(scene);
	//raycore::Scene::GLTF::load("../data/models/milkTruck/CesiumMilkTruck.gltf", scene);
	raycore::Scene::GLTF::load("../data/models/lantern/Lantern.gltf", scene);
	//raycore::Scene::GLTF::load("../data/models/duck/Duck.gltf", scene);
	//raycore::Scene::GLTF::load("../data/models/box/box.gltf", scene);
	//raycore::Scene::GLTF::load("../data/models/boxTextured/BoxTextured.gltf", scene);
	//raycore::Scene::GLTF::load("../data/models/sponza/Sponza.gltf", scene);
	raycore::Light l;
	l.position = vec3(0.f, 20.f, 0.f);
	scene.lights.push_back(l);
	application.run(scene, options);

	system("pause");
	return 1;
}