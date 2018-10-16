#include "SceneBuilder.h"


namespace raycore {


	SceneBuilder::SceneBuilder()
	{
	}


	SceneBuilder::~SceneBuilder()
	{
	}
	bool SceneBuilder::buildCustomScene(Scene &scene)
	{
		scene.materials.reserve(4);
		scene.shapes.reserve(4);
		scene.nodes.reserve(4);

		// materials
		Material &mat1 = scene.addMaterial();
		mat1.index = 0;
		mat1.color = colorHDR(0.9f);
		mat1.texture = nullptr;
		mat1.type = MaterialType::DIELECTRIC;

		Material &mat2 = scene.addMaterial();
		mat2.index = 1;
		mat2.color = colorHDR(0.5f, 1.f, 0.5f, 1.f);
		mat2.texture = nullptr;
		mat2.type = MaterialType::DIFFUSE;

		Material &mat3 = scene.addMaterial();
		mat3.index = 2;
		mat3.color = colorHDR(0.82f, 0.62f, 0.19f, 1.f);
		mat3.texture = nullptr;
		mat3.type = MaterialType::METAL;
		
		Texture *texture = scene.addTexture(new Checker(colorHDR(0.1f), colorHDR(1.f), 40, 40));
		Material &mat4 = scene.addMaterial();
		mat4.index = 3;
		mat4.color = colorHDR(0.7f, 0.7f, 0.7f, 1.f);
		mat4.texture = texture;
		mat4.type = MaterialType::DIFFUSE;

		// spheres
		Sphere &sphere1 = scene.addSphere();
		sphere1.center = point3(0.f);
		sphere1.radius = 0.4f;
		sphere1.up = vec3(0.f, 1.f, 0.f);
		sphere1.material = &mat1;

		Sphere &sphere2 = scene.addSphere();
		sphere2.center = point3(0.f, 0.1f, -1.0f);
		sphere2.radius = 0.5f;
		sphere2.up = vec3(0.f, 1.f, 0.f);
		sphere2.material = &mat2;

		Sphere &sphere3 = scene.addSphere();
		sphere3.center = point3(1.5f, 0.2f, -0.5f);
		sphere3.radius = 0.7f;
		sphere3.up = vec3(0.f, 1.f, 0.f);
		sphere3.material = &mat3;

		Sphere &sphere4 = scene.addSphere();
		sphere4.center = point3(0.f, -30.f, 0.f);
		sphere4.radius = 29.f;
		sphere4.up = vec3(1.f, 0.f, 0.f);
		sphere4.material = &mat4;

		// nodes
		Node &node1 = scene.addNode();
		node1.shape = &sphere1;
		node1.transform = mat4::identity();

		Node &node2 = scene.addNode();
		node2.shape = &sphere2;
		node2.transform = mat4::identity();

		Node &node3 = scene.addNode();
		node3.shape = &sphere3;
		node3.transform = mat4::identity();

		Node &node4 = scene.addNode();
		node4.shape = &sphere4;
		node4.transform = mat4::identity();

		return true;
	}
	bool SceneBuilder::buildCustomSceneTri(Scene &scene)
	{
		scene.materials.reserve(2);
		scene.shapes.reserve(2);
		scene.primitives.reserve(1);
		scene.nodes.reserve(2);

		// materials
		Material &mat1 = scene.addMaterial();
		mat1.index = 0;
		mat1.color = colorHDR(1.f, 1.f, 1.f, 1.f);
		mat1.texture = nullptr;
		mat1.type = MaterialType::DIFFUSE;
		mat1.index = 0;

		Material &mat2 = scene.addMaterial();
		mat2.index = 0;
		mat2.color = colorHDR(0.f, 1.f, 0.f, 1.f);
		mat2.texture = nullptr;
		mat2.type = MaterialType::DIFFUSE;
		mat2.index = 1;

		Mesh &mesh = scene.addMesh();
		Primitive &prim = scene.addPrimitive();
		Vertex vA = Vertex(point3(0.f), norm3(), uv2(0.f), colorHDR(1.f, 0.f, 0.f, 1.f));
		Vertex vB = Vertex(point3(0.f, 1.f, 0.f), norm3(), uv2(0.f), colorHDR(0.f, 1.f, 0.f, 1.f));
		Vertex vC = Vertex(point3(1.f, 0.f, 0.f), norm3(), uv2(0.f), colorHDR(0.f, 0.f, 1.f, 1.f));
		prim.vertices.push_back(vA);
		prim.vertices.push_back(vB);
		prim.vertices.push_back(vC);
		Triangle tri = Triangle(0, 2, 1);
		prim.triangles.push_back(tri);
		prim.material = &mat1;
		mesh.primitives.push_back(&prim);

		Sphere &sphere = scene.addSphere();
		sphere.radius = 0.8f;
		sphere.center = point3(0.f, 0.f, -1.f);
		sphere.material = &mat2;

		// nodes
		Node &node1 = scene.addNode();
		node1.shape = &mesh;
		node1.transform = mat4::identity();

		Node &node2 = scene.addNode();
		node2.shape = &sphere;
		node2.transform = mat4::identity();

		return true;
	}
}