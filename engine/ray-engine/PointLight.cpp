#include "PointLight.h"


namespace app {
	namespace tracer {

		PointLight::PointLight()
		{
		}


		PointLight::~PointLight()
		{
		}
		color4 PointLight::sample(LightInfo &lightInfo) const
		{
			return color4();
		}
		bool PointLight::hit(const prim::HitInfo & ray, LightInfo &lightInfo) const
		{
			// TODO
			// generate random ray within range, return PDF

			return false;
		}
	}
}