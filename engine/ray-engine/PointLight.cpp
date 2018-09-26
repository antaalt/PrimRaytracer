#include "PointLight.h"


namespace app {
	namespace tracer {

		PointLight::PointLight()
		{
		}


		PointLight::~PointLight()
		{
		}
		colorHDR PointLight::sample(LightInfo &lightInfo) const
		{
			return colorHDR();
		}
		bool PointLight::hit(const prim::HitInfo & ray, LightInfo &lightInfo) const
		{
			// TODO
			// generate random ray within range, return PDF

			return false;
		}
	}
}