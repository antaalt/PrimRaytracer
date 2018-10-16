#pragma once
#include "Hitable.h"

namespace raycore {
	namespace prim {
		class Sphere : public Hitable
		{
		public:
			Sphere();
			Sphere(const point3 &center, float radius, const vec3 up = vec3(0.f, 1.f, 0.f));
			~Sphere();

			virtual bool intersect(const tracer::Ray &ray, Intersection &intersection) const;
			virtual HitInfo computeIntersection(const tracer::Ray &ray, const Intersection &intersection) const;
			virtual BoundingBox computeBoundingBox() const;

		protected:
			float radius;
			point3 center;
			vec3 up;
		};
	}
}


