#include "Application.h"
#include "WhittedTracer.h"
#include "PathTracer.h"
#include "PinholeCamera.h"
#include "Loader.h"
#include "Sphere.h"
#include "OBJLoader.h"

#include <fstream>

void setScene(raycore::prim::Scene &scene)
{
	using namespace raycore;
	prim::Group *root = new prim::Group();
	root->setAcceleration(prim::Acceleration::BVH);

	prim::LightDistribution *ld = new prim::LightDistribution();
	//ld->addLight(new prim::ShapeLight(new prim::shape::Sphere(point3(0.f, 5.f, 0.f), 1.f), colorHDR(1.f), 1.f));
	//ld->addLight(new prim::ShapeLight(new prim::shape::Disk(point3(5.f, 2.f, 0.f), 1.f, norm3(0.f, 1.f, 0.f)), colorHDR(1.f), 1.f));
	//ld->addLight(new prim::PointLight(point3(5.f, 5.f, 0.f), colorHDR(1.f), 1.f));
	scene.setLightDistribution(ld);
	scene.setRoot(root);
	/*raycore::prim::Light *l;
	l.position = point3(0.f, 20.f, 0.f);
	scene.lights.push_back(l);*/

	Texture<colorHDR> *tex0 = scene.addTexture(new ConstantTexture<colorHDR>(colorHDR(0.9f)));
	Texture<colorHDR> *tex1 = scene.addTexture(new ConstantTexture<colorHDR>(colorHDR(0.5f, 1.f, 0.5f, 1.f)));
	Texture<colorHDR> *tex2 = scene.addTexture(new ConstantTexture<colorHDR>(colorHDR(0.82f, 0.62f, 0.19f, 1.f)));
	Texture<colorHDR> *tex3 = scene.addTexture(new Checker<colorHDR>(colorHDR(0.1f), colorHDR(1.f), 40, 40));
	Texture<colorHDR> *tex4 = scene.addTexture(new Checker<colorHDR>(colorHDR(0.5f, 0.5f, 1.f, 1.f), colorHDR(1.f, 0.5f, 0.5f, 1.f), 2, 2));
	Texture<float> *roughness = new ConstantTexture<float>(0.1f); // TODO leak HERE

	{
		// TODO replace vector by pointer array
		prim::Hitable* sphere = new prim::Sphere(point3(0.f), 0.4f, norm3(settings::up));
		sphere->material = scene.addMaterial(new prim::Glass(tex0, 1.1f));
		std::vector<prim::Hitable*> hitables(1, sphere);
		prim::Geometry *geom = new prim::Geometry(hitables);
		geom->setAcceleration(prim::Acceleration::NONE);
		root->addChild(geom);
	}
	{
		prim::Hitable* sphere = new prim::Sphere(point3(0.f, -1.0f, 0.1f), 0.5f, norm3(settings::up));
		sphere->material = scene.addMaterial(new prim::Matte(tex1));
		std::vector<prim::Hitable*> hitables(1, sphere);
		prim::Geometry *geom = new prim::Geometry(hitables);
		geom->setAcceleration(prim::Acceleration::NONE);
		root->addChild(geom);
	}
	{
		prim::Hitable* sphere = new prim::Sphere(point3(-1.5f, 0.f, 0.2f), 0.6f, norm3(settings::up));
		sphere->material = scene.addMaterial(new prim::Metal(tex2, roughness));
		std::vector<prim::Hitable*> hitables(1, sphere);
		prim::Geometry *geom = new prim::Geometry(hitables);
		geom->setAcceleration(prim::Acceleration::NONE);
		root->addChild(geom);
	}
	{
		prim::Hitable* sphere = new prim::Sphere(point3(0.f, 0.f, -30.f), 29.f, norm3(settings::up));
		sphere->material = scene.addMaterial(new  prim::Matte(tex3));
		std::vector<prim::Hitable*> hitables(1, sphere);
		prim::Geometry *geom = new prim::Geometry(hitables);
		geom->setAcceleration(prim::Acceleration::NONE);
		root->addChild(geom);
	}
	{
		prim::Hitable* sphere = new prim::Sphere(point3(-1.5f, 0.f, 0.2f), 0.6f, norm3(settings::up));
		sphere->material = scene.addMaterial(new prim::Matte(tex4));
		std::vector<prim::Hitable*> hitables(1, sphere);
		prim::Geometry *geom = new prim::Geometry(hitables);
		geom->setAcceleration(prim::Acceleration::NONE);
		root->addChild(geom);
	}
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
		//geom5->setTransform(mat4(
		//	mat4::col(15.f,  0.f,  0.f, 0.f),
		//	mat4::col( 0.f, 15.f,  0.f, 0.f),
		//	mat4::col( 0.f,  0.f, 15.f, 2.f),
		//	mat4::col( 0.f,  0.f,  0.f, 1.f)
		//));
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
	setScene(scene);
	application.run(scene, options);

	system("pause");
	return 1;
}