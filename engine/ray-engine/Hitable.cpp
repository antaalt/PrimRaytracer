#include "Hitable.h"


namespace app {
	namespace prim {
		bool Intersection::isClosestThan(const Intersection & intersection)
		{
			if (distance < intersection.distance)
				return true;
			distance = intersection.distance;
			beta = intersection.beta;
			gamma = intersection.gamma;
			hitable = intersection.hitable;
			return false;
		}
		HitInfo Intersection::compute(const tracer::Ray & ray)
		{
			return hitable->computeIntersection(ray, *this);
		}
		bool Intersection::hit()
		{
			return (hitable != nullptr);
		}
		Intersection::Intersection() : distance(INVALID_INTERSECTION), hitable(nullptr)
		{
		}
	}
}