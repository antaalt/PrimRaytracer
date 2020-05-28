#include "PointLight.h"


namespace raycore {
namespace prim {
PointLight::PointLight(const point3f & center, color4f albedo, float intensity) : Light(albedo, intensity), center(center)
{
}
bool PointLight::sample(const prim::HitInfo & info, const prim::Scene & scene, float * pdf, vec3f * sample) const
{
	vec3f s(center - info.point);
	if (scene.isOccluded(tracer::Ray(info.point, vec3f::normalize(s), tracer::GEOMETRY_RAY, EPSILON, s.norm())))
		return false;
	*sample = s;
	*pdf = 1.f;
	return true;
}
float PointLight::contribution(const prim::HitInfo & info) const
{
	float d = point3f::distance(info.point, center);
	return intensity / (d * d);
}
}
}

