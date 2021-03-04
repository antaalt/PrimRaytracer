#include "Scene.h"
#include "Triangle.h"
#include "Sphere.h"

namespace prim {

Scene::Scene()
{
}

Scene::~Scene()
{
	for (Light * light : lights)
		delete light;
	for (TransformNode * node : nodes)
		delete node;
	for (Material * material : materials)
		delete material;
	for (Texture4f * texture : textures)
		delete texture;
}

void Scene::build()
{
	for (TransformNode *node : this->nodes)
	{
		node->build();
		node->include(bbox);
	}
}

bool Scene::intersect(const Ray &ray, Intersection &intersection) const
{
	if (!bbox.intersect(ray))
		return false;
	bool hit = false;
	for (TransformNode *node : this->nodes) {
		if (node->intersect(ray, &intersection)) {
			hit = true;
		}
	}
	return hit;
}

}