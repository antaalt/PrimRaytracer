#pragma once

#include "Hitable.h"
#include "Types.h"

namespace app {
	namespace prim {

		struct Vertex {
			Point3 position;
			Normal normal;
			Texcoord texcoord;
			ColorHDR color;

			Vertex();
			Vertex(Point3 p, Normal n, Texcoord tc, ColorHDR color);
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

		};

	}
}