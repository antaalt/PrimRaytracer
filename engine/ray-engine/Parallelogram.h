#pragma once
#include "Hitable.h"
#include "Types.h"

namespace app {
	namespace prim {

		class Parallelogram : public Hitable
		{
		public:
			Parallelogram();
			Parallelogram(const point3 &A, const point3 &B, const point3 &C, const norm3 &normal);
			~Parallelogram();

			virtual bool intersect(const tracer::Ray &ray, Intersection &intersection) const;

			virtual HitInfo computeIntersection(const tracer::Ray &ray, const Intersection &intersection) const;
			virtual BoundingBox computeBoundingBox() const;

		private:
			point3 point[4];
			norm3 normal;
		};
	}
}

