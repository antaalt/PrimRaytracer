#pragma once

#include "Hitable.h"
#include "Config.h"

#include <array>

namespace raycore {

geometry::norm3f interpolate(const geometry::norm3f &nA, const geometry::norm3f &nB, const geometry::norm3f &nC, vec2f barycentric);
geometry::uv2f interpolate(const geometry::uv2f &tA, const geometry::uv2f &tB, const geometry::uv2f &tC, vec2f barycentric);
geometry::color4f interpolate(const geometry::color4f &cA, const geometry::color4f &cB, const geometry::color4f &cC, vec2f barycentric);

class Triangle : public Hitable
{
public:
	struct Vertex {
		geometry::point3f position;
		geometry::norm3f normal;
		geometry::uv2f texcoord;
		geometry::color4f color;
	};
public:
	bool intersect(const Ray &ray, Intersection *intersection) const override;

	ComputedIntersection compute(const Ray &ray, const Intersection &intersection) const override;
protected:
	float area() const;
private:
	union {
		Vertex data[3];
		struct {
			Vertex A, B, C;
		};
	};
};

}
