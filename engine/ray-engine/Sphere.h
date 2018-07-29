#pragma once
#include "Hitable.h"

namespace app {
	namespace prim {
		class Sphere : public Hitable
		{
		public:
			Sphere();
			Sphere(const Point3 &center, float radius);
			~Sphere();

			virtual bool intersect(const tracer::Ray &ray, Intersection &intersection) const;
			virtual HitInfo computeIntersection(const tracer::Ray &ray, const Intersection &intersection) const;

		private:
			float m_radius;
			Point3 m_center;
		};
	}
}


