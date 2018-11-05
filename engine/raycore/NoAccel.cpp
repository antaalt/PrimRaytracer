#include "NoAccel.h"

#include <map>


namespace raycore {
	namespace prim {
		NoAccel::NoAccel(const std::vector<Hitable*> &hitables) : Accelerator(hitables)
		{
			Log::info("Building NoAccel...");
		}
		NoAccel::~NoAccel()
		{
		}
		bool NoAccel::intersect(const tracer::Ray & ray, Intersection *intersection) const
		{
			if (!bbox.intersectBounds(ray))
				return false;
			// TODO compute size 
			for (size_t iHitable = 0; iHitable < this->hitables.size(); iHitable++)
			{
				Intersection localIntersection;
				if (this->hitables[iHitable]->intersect(ray, &localIntersection))
				{
					intersection->isClosestThan(localIntersection);
				}
			}
			return intersection->hit();
		}
		bool NoAccel::intersect(const tracer::Ray &ray) const
		{
			if (!bbox.intersectBounds(ray))
				return false;
			Intersection intersection;
			for (size_t iHitable = 0; iHitable < this->hitables.size(); iHitable++)
				if (this->hitables[iHitable]->intersect(ray, &intersection))
					return true;
			return false;
		}
	}
}

