#pragma once
#include "Config.h"
#include "Ray.h"
#include "Hitable.h"
#include "Accelerator.h"

namespace raycore {
	namespace tracer {

		class Light;

		struct LightInfo {
			vec3 sample;
			colorHDR color;
			float pdf;
			const Light *light;
		};

		class Light
		{
		public:
			Light(colorHDR albedo, float intensity);
			~Light();

			virtual bool sample(const prim::HitInfo &info, const Accelerator *accelerator, LightInfo &lightInfo) const = 0;

			float intensity;
			colorHDR albedo;
		};
	}
}

