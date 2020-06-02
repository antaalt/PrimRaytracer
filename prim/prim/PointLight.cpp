#include "PointLight.h"


namespace prim {

PointLight::PointLight(const point3f & center, color4f albedo, float intensity) : Light(albedo, intensity), center(center)
{
}
bool PointLight::sample(const ComputedIntersection & info, const Scene & scene, float * pdf, vec3f * sample) const
{
	vec3f s(center - info.point);
	BackCulling culling;
	Intersection intersection(culling, true);
	Ray shadowRay(info.point, vec3f::normalize(s), EPSILON, s.norm());
	if (scene.intersect(shadowRay, intersection))
		return false;
	*sample = s;
	*pdf = 1.f;
	return true;
}
float PointLight::contribution(const ComputedIntersection & info) const
{
	float d = point3f::distance(info.point, center);
	return m_intensity / (d * d);
}
}


