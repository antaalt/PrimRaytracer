#pragma once

#include "Hitable.h"
#include "Config.h"

#define MOLLER_TRUMBORE
//#define BACKFACE_CULLING

namespace raycore {
	namespace prim {

		struct Vertex {
			point3 position;
			norm3 normal;
			uv2 texcoord;
			colorHDR color;

			Vertex();
			Vertex(point3 p, norm3 n, uv2 tc, colorHDR color);
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
