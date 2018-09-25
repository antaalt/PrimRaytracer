#pragma once
#include "Hitable.h"
#include "Scene.h"
#include "BoundingBox.h"
#include "Light.h"

namespace app {
	namespace tracer {

		enum class Acceleration {
			OCTREE,
			BVH,
			NONE
		};

		class Accelerator
		{
		public:
			using Ptr = Accelerator*;
			Accelerator();
			~Accelerator();

			virtual bool build(const Scene &scene);

			virtual bool intersect(const Ray &ray, prim::HitInfo &info) const = 0;

			virtual bool isOccluded(const Ray &ray) const = 0;

			const Light *getLight(size_t index) const;

			size_t getLightsCount() const;

		protected:
			prim::BoundingBox bbox;						// Bounding box of the whole acceleration structure
			std::vector<prim::Hitable*> hitables;		// Hitable inside the acceleration structure
			std::vector<prim::Material*> materials;		// materials of the hitables
			std::vector<Texture> textures;				// Textures of the hitables
			std::vector<Light*> lights;					// Lights of the scene
			size_t hitableCount;
		};
	}
}