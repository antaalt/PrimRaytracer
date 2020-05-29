#include "PointLight.h"


namespace raycore {

PointLight::PointLight(const point3f & center, color4f albedo, float intensity) : Light(albedo, intensity), center(center)
{
}
bool PointLight::sample(const ComputedIntersection & info, const Scene & scene, float * pdf, vec3f * sample) const
{
	vec3f s(center - info.point);
	ComputedIntersection info2;
	if (scene.intersect(Ray(info.point, vec3f::normalize(s), EPSILON, s.norm()), &info2))
		return false;
	*sample = s;
	*pdf = 1.f;
	return true;
}
float PointLight::contribution(const ComputedIntersection & info) const
{
	float d = point3f::distance(info.point, center);
	return intensity / (d * d);
}
}


