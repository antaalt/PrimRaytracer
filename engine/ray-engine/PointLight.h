#pragma once
#include "Light.h"
namespace app {
	namespace tracer {
		class PointLight : public Light
		{
		public:
			PointLight();
			~PointLight();

			virtual color4 sample(LightInfo &lightInfo) const;

			virtual bool hit(const prim::HitInfo &ray, LightInfo &lightInfo) const;

		private:
			vec3 position;
		};
	}
}
