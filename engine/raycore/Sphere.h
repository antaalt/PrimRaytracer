#pragma once
#include "Hitable.h"

namespace raycore {
	namespace prim {
		class Sphere : public Hitable
		{
		public:
			Sphere();
			Sphere(const point3 &center, float radius, const norm3 up = norm3(settings::up));
			~Sphere();

			virtual bool intersect(const tracer::Ray &ray, Intersection *intersection) const;
			virtual HitInfo computeIntersection(const tracer::Ray &ray, const Intersection *intersection) const;
			virtual BoundingBox computeBoundingBox() const;
			virtual float area() const;
		protected:
			float radius;
			point3 center;
			norm3 up;
		};
	}
}


