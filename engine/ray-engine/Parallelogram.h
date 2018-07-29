#pragma once
#include "Hitable.h"
#include "Types.h"

namespace app {
	namespace prim {

		class Parallelogram : public Hitable
		{
		public:
			Parallelogram();
			Parallelogram(const Point3 &A, const Point3 &B, const Point3 &C, const Normal &normal);
			~Parallelogram();

			virtual bool intersect(const tracer::Ray &ray, Intersection &intersection) const;

			virtual HitInfo computeIntersection(const tracer::Ray &ray, const Intersection &intersection) const;

		private:
			Point3 point[4];
			Normal normal;
		};
	}
}

