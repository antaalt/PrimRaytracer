#pragma once
#include "Config.h"
#include "Ray.h"
#include "Accelerator.h"

namespace app {
	namespace tracer {
		class Light
		{
		public:
			Light();
			~Light();

			virtual color4 sample() = 0;
			bool inShadow(const Ray &shadowRay, Accelerator *acc);

			float intensity;
			color4 albedo;
		};
	}
}

