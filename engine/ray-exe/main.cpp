#include "WhittedTracer.h"
#include "PinholeCamera.h"
#include "SceneBuilder.h"
#include "Renderer.h"
#include "stb_image_write.h"


int main(int argc, char *argv[])
{
	unsigned int width = 480;
	unsigned int height = 360;

	raycore::tracer::Tracer::Ptr tracer = new raycore::tracer::WhittedTracer();
	raycore::tracer::Camera::Ptr camera = new raycore::tracer::PinholeCamera(width, height);
	camera->lookAt(point3(
		0.f,
		0.f,
		1.f
	), point3(0.f));

	// Load Scene
	raycore::Scene scene;
	raycore::SceneBuilder::buildCustomScene(scene);
	//app::Scene scene = app::Scene::GLTF::load("../data/models/box/box.gltf");
	//app::Scene scene = app::Scene::GLTF::load("../data/models/boxTextured/BoxTextured.gltf");

	raycore::tracer::Renderer renderer(width, height, 32);
	renderer.setCamera(camera);
	renderer.setTracer(tracer);
	renderer.buildScene(std::move(scene), raycore::tracer::Acceleration::NONE);
	if (renderer.render())
	{
		const raycore::Pixel *data = renderer.image().data();
		stbi_write_hdr("render.hdr", width, height, 4, data->data);
		Log::info("Image saved to output.hdr");
		return 1;
	}
	throw std::runtime_error("Could not render image");
}