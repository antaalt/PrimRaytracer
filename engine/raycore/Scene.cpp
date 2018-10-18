#include "Scene.h"

#include "Config.h"

namespace raycore {
	Triangle::Triangle()
	{
	}
	Triangle::Triangle(unsigned int v1, unsigned int v2, unsigned int v3): A(v1), B(v2), C(v3)
	{
	}
	Scene::Scene()
	{
	}
	Scene::~Scene()
	{
		for (Shape* shape : shapes)
			delete shape;
		for (Texture<colorHDR>* texture : textures)
			delete texture;
	}
	Mesh & Scene::addMesh()
	{
		Mesh *m = new Mesh();
		shapes.push_back(m);
		return *m;
	}
	Sphere & Scene::addSphere()
	{
		Sphere *s = new Sphere();
		shapes.push_back(s);
		return *s;
	}
	Parallelogram & Scene::addParallelogram()
	{
		Parallelogram *p = new Parallelogram();
		shapes.push_back(p);
		return *p;
	}
	Node & Scene::addNode()
	{
		nodes.emplace_back();
		return nodes.back();
	}
	Material & Scene::addMaterial()
	{
		materials.emplace_back();
		return materials.back();
	}
	Texture<colorHDR> * Scene::addTexture(Texture<colorHDR>* texture)
	{
		textures.push_back(texture);
		return textures.back();
	}
	Light & Scene::addLight()
	{
		lights.emplace_back();
		return lights.back();
	}
	mat4 Node::getModel() const
	{
		if (parent == nullptr)
			return transform;
		return parent->getModel() * transform;
	}
	ShapeType Sphere::type() const
	{
		return ShapeType::SPHERE;
	}
	ShapeType Mesh::type() const
	{
		return ShapeType::MESH;
	}
	ShapeType Parallelogram::type() const
	{
		return ShapeType::PARALLELOGRAM;
	}
}

