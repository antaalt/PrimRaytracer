#pragma once
#include "Accelerator.h"

#include "Material.h"
#include "BoundingBox.h"

namespace raycore {
	namespace tracer {

		class NoAccel : public Accelerator
		{
		public:
			NoAccel();
			~NoAccel();

			virtual bool intersect(const Ray &ray, prim::HitInfo &intersection) const;

			virtual bool isOccluded(const Ray &ray) const;
		};
	}
}


