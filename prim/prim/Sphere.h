#pragma once
#include "Hitable.h"

namespace prim {

class Sphere : public Hitable
{
public:
	Sphere(const mat4f &transform, float radius, Material *material);

	bool intersect(const Ray &ray, Intersection *intersection) const override;

	ComputedIntersection compute(const Ray &ray, const Intersection &intersection) const override;

protected:
	float area() const override;
protected:
	float m_radius;
};

}


