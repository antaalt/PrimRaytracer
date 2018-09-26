#pragma once
#include "Config.h"
#include "Ray.h"
#include "Hitable.h"

namespace app {
	namespace tracer {

		struct LightInfo {
			vec3 raySample;
		};

		class Light : public prim::Hitable
		{
		public:
			Light();
			~Light();

			virtual colorHDR shade(LightInfo &lightInfo, float &pdf) const = 0;

			virtual bool hit(const prim::HitInfo &info, LightInfo &lightInfo) const = 0;

			virtual prim::BoundingBox computeBBox() const { return prim::BoundingBox(); }

			float intensity;
			colorHDR albedo;
		};
	}
}

