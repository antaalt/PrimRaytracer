#pragma once

#include "Hitable.h"
#include "Config.h"

#include <array>

namespace prim {

class Triangle : public Hitable {
public:
	struct Vertex {
		point3f position;
		norm3f normal;
		uv2f texcoord;
	};
public:
	Triangle(Material *material);
	Triangle(const Vertex &A, const Vertex &B, const Vertex &Cs, Material *material);

	bool intersect(const Ray &ray, Intersection *intersection) const override;
	void include(BoundingBox &boundingBox) const override;

	static norm3f interpolate(const norm3f &nA, const norm3f &nB, const norm3f &nC, vec2f barycentric);
	static uv2f interpolate(const uv2f &tA, const uv2f &tB, const uv2f &tC, vec2f barycentric);
	
	float area() const;
	point3f center() const;

	union {
		Vertex vertices[3];
		struct {
			Vertex A, B, C;
		};
	};
};

}
