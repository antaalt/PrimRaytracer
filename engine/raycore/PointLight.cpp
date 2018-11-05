#include "PointLight.h"


namespace raycore {
	namespace prim {
		PointLight::PointLight(const point3 & center, colorHDR albedo, float intensity) : Light(albedo, intensity), center(center)
		{
		}
		bool PointLight::sample(const prim::HitInfo & info, const prim::Scene & scene, float * pdf, vec3 * sample) const
		{
			vec3 s(center - info.point);
			if (scene.isOccluded(tracer::Ray(info.point, normalize(s), tracer::GEOMETRY_RAY, EPSILON, length(s))))
				return false;
			*sample = s;
			*pdf = 1.f;
			return true;
		}
		float PointLight::contribution(const prim::HitInfo & info) const
		{
			float d = distance(info.point, center);
			return intensity / (d * d);
		}
	}
}

