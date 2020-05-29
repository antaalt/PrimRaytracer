#include "Scene.h"
#include "Triangle.h"
#include "Sphere.h"

namespace raycore {

void Scene::build()
{
	for (Hitable *hitable : this->hitables)
	{
		// TODO fill bbox
		hitable->build();
	}
}
bool Scene::intersect(const Ray &ray, ComputedIntersection *info) const
{
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