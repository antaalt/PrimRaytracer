#include "Application.h"
#include "WhittedTracer.h"
#include "PinholeCamera.h"
#include "SceneBuilder.h"
#include "stb_image_write.h"

int renderSingleFrame()
{
	unsigned int width = 480;
	unsigned int height = 360;

	app::tracer::Tracer::Ptr tracer = new app::tracer::WhittedTracer();
	app::tracer::Camera::Ptr camera = new app::tracer::PinholeCamera(width, height);
	camera->lookAt(point3(
		0.f,
		0.f,
		1.f
	), point3(0.f));

	// Load Scene
	app::Scene scene = app::SceneBuilder::buildCustomScene();
	//app::Scene scene = app::Scene::GLTF::load("../data/box/box.gltf");
	//app::Scene scene = app::Scene::GLTF::load("../data/boxTextured/BoxTextured.gltf");

	app::tracer::Renderer renderer(width, height, 32);
	renderer.setCamera(camera);
	renderer.setTracer(tracer);
	renderer.buildScene(std::move(scene), app::tracer::Acceleration::NO_ACCEL);
	if (renderer.render())
	{
		const app::Pixel *data = renderer.image().data();
		stbi_write_hdr("render.hdr", width, height, 4, data->data);
		Log::info("Image saved to output.hdr");
		return 1;
	}
	throw std::runtime_error("Could not render image");
}

int renderDisplay()
{
	unsigned int width = 480;
	unsigned int height = 360;
	app::Application application(width, height);
	// Set options
	app::options options;
	options.tracer = new app::tracer::WhittedTracer();
	options.camera = new app::tracer::PinholeCamera(width, height);
	options.camera->lookAt(point3(
		0.f,
		5.f,
		10.f
	), point3(0.f, 5.f, 0.f));
	options.acceleration = app::tracer::Acceleration::ACCELERATION_OCTREE;

	// Load Scene
	Log::info("Loading scene");
	//app::Scene scene = app::SceneBuilder::buildCustomScene();
	app::Scene scene = app::Scene::GLTF::load("../data/lantern/Lantern.gltf");
	//app::Scene scene = app::Scene::GLTF::load("../data/duck/Duck.gltf");
	//app::Scene scene = app::Scene::GLTF::load("../data/box/box.gltf");
	//app::Scene scene = app::Scene::GLTF::load("../data/boxTextured/BoxTextured.gltf");
	//app::Scene scene = app::Scene::GLTF::load("../data/sponza/Sponza.gltf");

	application.run(scene, options);

	system("pause");
	return 1;
}

int main(int argc, char *argv[])
{
	return renderDisplay();
}