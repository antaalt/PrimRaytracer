#include "Atmosphere.h"

namespace app {

	namespace prim {

		Atmosphere::Atmosphere()
		{
		}


		Atmosphere::~Atmosphere()
		{
		}
		HitInfo Atmosphere::computeIntersection(const tracer::Ray & ray, const Intersection & intersection) const
		{
			return HitInfo();
		}
	}
}