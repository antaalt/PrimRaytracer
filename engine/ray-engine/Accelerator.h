#pragma once
#include "Hitable.h"
#include "Scene.h"
#include "BoundingBox.h"

namespace app {
	namespace tracer {

		enum class Acceleration {
			ACCELERATION_OCTREE,
			ACCELERATION_BVH,
			NO_ACCEL
		};

		class Accelerator
		{
		public:
			using Ptr = Accelerator*;
			Accelerator();
			~Accelerator();

			virtual bool build(const Scene &scene);

			virtual bool intersect(const Ray &ray, prim::HitInfo &info) const = 0;

			virtual bool intersect(const Ray &ray) const = 0;

		protected:
			prim::BoundingBox bbox;						// Bounding box of the whole acceleration structure
			std::vector<prim::Hitable*> hitables;		// Hitable inside the acceleration structure
			std::vector<prim::Material*> materials;		// materials of the hitables
			std::vector<Texture> textures;				// Textures of the hitables
			size_t hitableCount;
		};
	}
}