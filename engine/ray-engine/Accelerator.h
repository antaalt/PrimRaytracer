#pragma once
#include "Hitable.h"
#include "Scene.h"

namespace app {
	namespace tracer {

		enum class Acceleration {
			ACCELERATION_OCTREE,
			ACCELERATION_BOUNDING_BOX,
			NO_ACCEL
		};

		class Accelerator
		{
		public:
			using Ptr = Accelerator*;
			Accelerator();
			~Accelerator();

			virtual bool build(const Scene &scene) = 0;

			virtual bool intersect(const Ray &ray, prim::HitInfo &info) const = 0;
		};
	}
}