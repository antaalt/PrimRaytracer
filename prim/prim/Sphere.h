#pragma once
#include "Hitable.h"

namespace prim {

class Sphere : public Hitable
{
public:
	Sphere(const mat4f &transform, float radius, Material *material);

	bool intersect(const Ray &ray, Intersection &intersection) const override;

	void compute(const point3f &hitPoint, const vec2f &barycentric, Intersection::Indice indice, norm3f *normal, uv2f *texCoord, color4f *color) const override;

	void include(BoundingBox &boundingBox) override;
protected:
	float area() const;
protected:
	float m_radius;
};

}


