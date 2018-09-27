#pragma once
#include "Light.h"
namespace raycore {
	namespace tracer {
		class PointLight : public Light
		{
		public:
			PointLight(colorHDR albedo, float intensity, point3 position);
			~PointLight();

			virtual bool sample(const prim::HitInfo &info, const Accelerator *accelerator, LightInfo &lightInfo) const;

		private:
			point3 position;
		};
	}
}
