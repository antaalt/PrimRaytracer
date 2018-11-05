#include "Application.h"
#include "WhittedTracer.h"
#include "PathTracer.h"
#include "PinholeCamera.h"
#include "Loader.h"

int main(int argc, char *argv[])
{
	unsigned int width = 800;
	unsigned int height = 600;
	app::Application application(width, height);
	// Set options
	app::options options;
	options.tracer = new raycore::tracer::PathTracer();
	options.camera = new raycore::tracer::PinholeCamera(0.f, 0.f);
	/*options.camera->lookAt(point3(
		1.f,
		1.f,
		0.f
	), point3(0.f, 2.f, 0.f));*/
	options.camera->lookAt(point3(
		0.f,
		4.f,
		4.f
	), point3(0.f, 0.f, 0.f));
	options.acceleration = raycore::prim::Acceleration::BVH;
	options.settings.raySamplerX = raycore::tracer::RaySampler::RANDOM;
	options.settings.raySamplerY = raycore::tracer::RaySampler::RANDOM;
	options.settings.samplesX = 1;
	options.settings.samplesY = 1;
	options.settings.tileSize = 32;

	// Load Scene
	Log::info("Loading scene");
	raycore::prim::Scene scene;
	raycore::builder::newBuild(scene);
	//raycore::GLTF::load("../data/models/milkTruck/CesiumMilkTruck.gltf", scene);
	//raycore::GLTF::load("../data/models/lantern/Lantern.gltf", scene);
	//raycore::GLTF::load("../data/models/duck/Duck.gltf", scene);
	//raycore::GLTF::load("../data/models/box/box.gltf", scene);
	//raycore::GLTF::load("../data/models/boxTextured/BoxTextured.gltf", scene);
	//raycore::GLTF::load("../data/models/sponza/Sponza.gltf", scene);
	/*raycore::prim::Light *l;
	l.position = point3(0.f, 20.f, 0.f);
	scene.lights.push_back(l);*/
	application.run(scene, options);

	system("pause");
	return 1;
}