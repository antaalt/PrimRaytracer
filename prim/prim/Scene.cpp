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
bool Scene::intersect(const Ray &ray, ComputedIntersection *info) const
{
	if (!bbox.intersect(ray))
		return false;
	Intersection intersection;
	for (Hitable *hitable : this->hitables)
		hitable->intersect(ray, &intersection);
	if (intersection.valid())
	{
		const Hitable *hitable = intersection.hitable;
		*info = hitable->compute(ray, intersection);
		return true;
	}
	return false;
}

}