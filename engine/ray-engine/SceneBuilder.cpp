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
		scene.materials.reserve(4);
		scene.shapes.reserve(4);
		scene.nodes.reserve(4);

		// materials
		Material &mat1 = scene.addMaterial();
		mat1.index = 0;
		mat1.color = ColorHDR(1.f, 0.f, 0.f, 1.f);
		mat1.texture = nullptr;
		mat1.type = MaterialType::DIELECTRIC;

		Material &mat2 = scene.addMaterial();
		mat2.index = 1;
		mat2.color = ColorHDR(0.f, 1.f, 0.f, 1.f);
		mat2.texture = nullptr;
		mat2.type = MaterialType::DIFFUSE;

		Material &mat3 = scene.addMaterial();
		mat3.index = 2;
		mat3.color = ColorHDR(0.f, 0.f, 1.f, 1.f);
		mat3.texture = nullptr;
		mat3.type = MaterialType::SPECULAR;

		Material &mat4 = scene.addMaterial();
		mat4.index = 3;
		mat4.color = ColorHDR(1.f, 1.f, 1.f, 1.f);
		mat4.texture = nullptr;
		mat4.type = MaterialType::DIFFUSE;

		// spheres
		Sphere &sphere1 = scene.addSphere();
		sphere1.center = Point3(0.f);
		sphere1.radius = 4.f;
		sphere1.material = &mat1;

		Sphere &sphere2 = scene.addSphere();
		sphere2.center = Point3(0.f, 1.f, -10.f);
		sphere2.radius = 6.f;
		sphere2.material = &mat2;

		Sphere &sphere3 = scene.addSphere();
		sphere3.center = Point3(15.f, 2.f, -5.f);
		sphere3.radius = 7.f;
		sphere3.material = &mat3;

		// Planes
		Parallelogram &para = scene.addParallelogram();
		para.material = &mat4;
		para.point[0] = Point3(-10.f, 0.f, 0.f);
		para.point[1] = Point3();
		para.point[2] = Point3();
		para.normal = Normal(1.f, 0.f, 0.f);

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

		Node &node4 = scene.addNode();
		node4.shape = &para;
		node4.transform = Matrix4::identity();

		return scene;
	}
}