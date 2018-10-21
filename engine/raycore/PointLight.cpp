#include "PointLight.h"


namespace raycore {
	namespace tracer {

		PointLight::PointLight(colorHDR albedo, float intensity, point3 position) : Light(albedo, intensity), position(position)
		{
		}

		PointLight::~PointLight()
		{
		}

		bool PointLight::sample(const prim::HitInfo & info, const Accelerator *accelerator, LightInfo &lightInfo) const
		{
			vec3 lightDir(this->position - info.point);
			float dist = length(lightDir);
			lightDir = normalize(lightDir);
			Ray shadowRay(info.point, lightDir, tracer::RayType::SHADOW_RAY, EPSILON, dist);

			if (accelerator->isOccluded(shadowRay))
				return false;
			lightInfo.sample = lightDir;
			lightInfo.light = this;
			lightInfo.color = this->albedo * this->intensity;
			lightInfo.pdf = 1.f;
			return true;
		}
	}
}