#include "Scene.h"
#include "Triangle.h"
#include "Sphere.h"

namespace prim {

void Scene::build()
{
	for (Hitable *hitable : this->hitables)
	{
		hitable->build();
		hitable->include(bbox);
	}
}
bool Scene::intersect(const Ray &ray, Intersection &intersection) const
{
	if (!bbox.intersect(ray))
		return false;
	for (Hitable *hitable : this->hitables)
		hitable->intersect(ray, intersection);
	return intersection.valid();
}

}