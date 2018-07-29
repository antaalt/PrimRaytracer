#include "SceneBuilder.h"


namespace app {


	SceneBuilder::SceneBuilder()
	{
	}


	SceneBuilder::~SceneBuilder()
	{
	}
	Scene SceneBuilder::buildCustomScene()
	{
		Scene scene;
		scene.materials.reserve(3);
		scene.shapes.reserve(3);
		scene.nodes.reserve(3);

		// materials
		Material &mat1 = scene.addMaterial();
		mat1.index = 0;
		mat1.color = ColorHDR(1.f, 0.f, 0.f, 1.f);
		mat1.texture = nullptr;
		mat1.type = MaterialType::DIFFUSE;

		Material &mat2 = scene.addMaterial();
		mat2.index = 1;
		mat2.color = ColorHDR(0.f, 1.f, 0.f, 1.f);
		mat2.texture = nullptr;
		mat1.type = MaterialType::DIELECTRIC;

		Material &mat3 = scene.addMaterial();
		mat3.index = 2;
		mat3.color = ColorHDR(0.f, 0.f, 1.f, 1.f);
		mat3.texture = nullptr;
		mat1.type = MaterialType::SPECULAR;

		// spheres
		Sphere &sphere1 = scene.addSphere();
		sphere1.center = Point3(0.f);
		sphere1.radius = 4.f;
		sphere1.material = &mat1;

		Sphere &sphere2 = scene.addSphere();
		sphere2.center = Point3(0.f, 1.f, -5.f);
		sphere2.radius = 6.f;
		sphere2.material = &mat2;

		Sphere &sphere3 = scene.addSphere();
		sphere3.center = Point3(15.f, 2.f, -5.f);
		sphere3.radius = 7.f;
		sphere3.material = &mat3;

		// nodes
		Node &node1 = scene.addNode();
		node1.shape = &sphere1;
		node1.transform = Matrix4::identity();

		Node &node2 = scene.addNode();
		node2.shape = &sphere2;
		node2.transform = Matrix4::identity();

		Node &node3 = scene.addNode();
		node3.shape = &sphere3;
		node3.transform = Matrix4::identity();

		return scene;
	}
}