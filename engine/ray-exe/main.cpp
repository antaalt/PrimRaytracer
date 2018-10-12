#include "PathTracer.h"
#include "PinholeCamera.h"
#include "SceneBuilder.h"
#include "Renderer.h"
#include "stb_image_write.h"
#include "Mathematic.h"

int main(int argc, char *argv[])
{
	unsigned int width = 480;
	unsigned int height = 360;

	raycore::tracer::Tracer* tracer = new raycore::tracer::PathTracer();
	raycore::tracer::Camera* camera = new raycore::tracer::PinholeCamera();
	camera->lookAt(point3(
		0.f,
		4.f,
		4.f
	), point3(0.f));

	// Load Scene
	raycore::Scene scene;
	raycore::SceneBuilder::buildCustomScene(scene);
	//app::Scene scene = app::Scene::GLTF::load("../data/models/box/box.gltf");
	//app::Scene scene = app::Scene::GLTF::load("../data/models/boxTextured/BoxTextured.gltf");
	raycore::tracer::Settings settings;
	settings.raySamplerX = raycore::tracer::RaySampler::RANDOM;
	settings.raySamplerY = raycore::tracer::RaySampler::RANDOM;
	settings.samplesX = 1;
	settings.samplesY = 1;
	settings.tileSize = 32;
	raycore::tracer::Renderer renderer(width, height, settings);
	renderer.setCamera(camera);
	renderer.setTracer(tracer);
	renderer.buildScene(std::move(scene), raycore::tracer::Acceleration::BVH);
	if (renderer.render())
	{
		const raycore::Pixel *data = renderer.image().data();
		std::vector<unsigned char> output(width * height * 3);
		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				output[y * width * 3 + x * 3 + 0] = static_cast<unsigned char>(raycore::clamp(data[y * width + x].r * 255.f, 0.f, 255.f));
				output[y * width * 3 + x * 3 + 1] = static_cast<unsigned char>(raycore::clamp(data[y * width + x].g * 255.f, 0.f, 255.f));
				output[y * width * 3 + x * 3 + 2] = static_cast<unsigned char>(raycore::clamp(data[y * width + x].b * 255.f, 0.f, 255.f));
			}
		}
		stbi_flip_vertically_on_write(true);
		stbi_write_jpg("output.jpg", width, height, 3, output.data(), 80);
		Log::info("Image saved to output.hdr");
		return 1;
	}
	throw std::runtime_error("Could not render image");
}