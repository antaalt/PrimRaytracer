#pragma once
#include "Accelerator.h"

#include "Material.h"
#include "BoundingBox.h"

namespace raycore {
	namespace prim {

		class NoAccel : public Accelerator
		{
		public:
			NoAccel(const std::vector<Hitable*> &hitables);
			~NoAccel();

			virtual bool intersect(const tracer::Ray &ray, Intersection *intersection) const;
			virtual bool intersect(const tracer::Ray &ray) const;
		};
	}
}


