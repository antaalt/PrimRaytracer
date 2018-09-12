#include "NoAccel.h"

#include "Triangle.h"
#include "Sphere.h"
#include "Parallelogram.h"

#include <map>


namespace app {
	namespace tracer {
		NoAccel::NoAccel()
		{
		}
		NoAccel::~NoAccel()
		{
		}
		bool NoAccel::intersect(const Ray & ray, prim::HitInfo & info) const
		{
			if (!bbox.intersectBounds(ray))
				return false;
			prim::Intersection intersection;
			for (size_t iHitable = 0; iHitable < this->hitableCount; iHitable++)
			{
				prim::Intersection localIntersection;
				if (this->hitables[iHitable]->intersect(ray, localIntersection))
				{
					intersection.isClosestThan(localIntersection);
				}
			}
			if (!intersection.hit())
				return false;
			
			info = intersection.compute(ray);
			return true;
		}
		bool NoAccel::intersect(const Ray & ray) const
		{
			if (!bbox.intersectBounds(ray))
				return false;
			prim::Intersection intersection;
			for (size_t iHitable = 0; iHitable < this->hitableCount; iHitable++)
				if (this->hitables[iHitable]->intersect(ray, intersection))
					return true;
			return false;
		}
	}
}

