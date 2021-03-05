#pragma once
#include "../../Core/Hitable.h"

namespace prim {

class Sphere : public Hitable
{
public:
	Sphere(float radius, Material *material);

	bool intersect(const Ray &ray, Intersection *intersection) const override;

	void include(BoundingBox &boundingBox) const override;

	float getRadius() const;
protected:
	float area() const;
protected:
	float m_radius;
};

}


