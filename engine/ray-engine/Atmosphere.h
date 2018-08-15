#pragma once
#include "Sphere.h"
#include "Time.h"
#include "Location.h"

namespace app {

	namespace prim {

		

		class Atmosphere : public Sphere
		{
		public:
			Atmosphere();
			~Atmosphere();
			// sky dome, compute rayleigh and co
			virtual HitInfo computeIntersection(const tracer::Ray &ray, const Intersection &intersection) const;

		private:
			Time m_time;
			Location m_location;
		};
	}
}

