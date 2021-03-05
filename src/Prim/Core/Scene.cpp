#include "Scene.h"

namespace prim {

Scene::Scene()
{
}

Scene::~Scene()
{
	for (Light * light : lights)
		delete light;
	for (Hitable * node : nodes)
		delete node;
	for (Material * material : materials)
		delete material;
	for (Texture4f * texture : textures)
		delete texture;
}

void Scene::build()
{
	for (Hitable* node : this->nodes)
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
	for (Hitable* node : this->nodes) {
		hit |= node->intersect(ray, &intersection);
	}
	return hit;
}

}