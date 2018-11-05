#pragma once
#include "Config.h"
#include "Ray.h"
#include "Hitable.h"
#include "Scene.h"

namespace raycore {
	namespace prim {

		struct Scene;
		
		class Light
		{
		public:
			Light(colorHDR albedo, float intensity);
			virtual bool sample(const HitInfo &info, const Scene &scene, float *pdf, vec3 *sample) const = 0;
			virtual float contribution(const prim::HitInfo &info) const = 0;
			virtual colorHDR shade() const
			{
				return albedo * intensity;
			}
		protected:
			float intensity;
			colorHDR albedo;
		};
	}
}

