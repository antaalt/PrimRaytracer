#pragma once

#include "Hitable.h"
#include "Types.h"

#define MOLLER_TRUMBORE
//#define BACKFACE_CULLING

namespace app {
	namespace prim {

		struct Vertex {
			point3 position;
			norm3 normal;
			uv2 texcoord;
			color4 color;

			Vertex();
			Vertex(point3 p, norm3 n, uv2 tc, color4 color);
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
			~Triangle();

			virtual bool intersect(const tracer::Ray &ray, Intersection &intersection) const;
			virtual HitInfo computeIntersection(const tracer::Ray &ray, const Intersection &intersection) const;
			virtual BoundingBox computeBoundingBox() const;

		};

	}
}
