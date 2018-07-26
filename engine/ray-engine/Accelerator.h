#pragma once
#include "Hitable.h"
#include "Scene.h"

namespace Application {
	namespace RayTracer {

		enum class Acceleration {
			ACCELERATION_OCTREE,
			ACCELERATION_BOUNDING_BOX,
			NO_ACCEL
		};

		class Accelerator : public Hitable
		{
		public:
			Accelerator();
			~Accelerator();

			virtual bool build(const Scene &scene) = 0;

			virtual bool intersect(const Ray &ray, Intersection &intersection) const = 0;
		};
	}
}