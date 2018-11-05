#pragma once
#include "Hitable.h"
#include "BoundingBox.h"

#include <vector>

namespace raycore {
	namespace prim {

		class Light;

		enum class Acceleration {
			OCTREE,
			BVH,
			NONE
		};
		
		class Accelerator
		{
		public:
			Accelerator(const std::vector<Hitable*> &prim);

			virtual bool intersect(const tracer::Ray &ray, Intersection *intersection) const = 0;
			virtual bool intersect(const tracer::Ray &ray) const = 0;
		protected:
			BoundingBox bbox;
			std::vector<Hitable*> hitables;
		};
	}
}