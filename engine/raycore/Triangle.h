#pragma once

#include "Hitable.h"
#include "Config.h"

#define MOLLER_TRUMBORE
//#define BACKFACE_CULLING

namespace raycore {
namespace prim {

struct Vertex {
	geometry::point3f position;
	geometry::norm3f normal;
	geometry::uv2f texcoord;
	geometry::color4f color;

	Vertex();
	Vertex(geometry::point3f p, geometry::norm3f n, geometry::uv2f tc, geometry::color4f color);
};

class Triangle : public Hitable
{
public:
	union {
		Vertex vertices[3];
		struct {
			Vertex A, B, C;
		};
	};

	Triangle();
	Triangle(const Vertex &vA, const Vertex &vB, const Vertex &vC);

	virtual bool intersect(const tracer::Ray &ray, Intersection *intersection) const;
	virtual HitInfo computeIntersection(const tracer::Ray &ray, const Intersection *intersection) const;
	virtual BoundingBox computeBoundingBox() const;
	virtual float area() const;
};

}
}
