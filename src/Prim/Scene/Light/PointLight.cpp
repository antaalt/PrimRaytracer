#include "PointLight.h"

namespace prim {

PointLight::PointLight(const point3f & center, color4f albedo, float intensity) : Light(albedo, intensity), center(center)
{
}

Ray PointLight::sample(const point3f & hitPoint) const
{
	Ray ray;
	ray.origin = hitPoint;
	ray.direction = vec3f(center - hitPoint);
	ray.tmax = ray.direction.norm();
	return ray;
}

float PointLight::pdf(const Ray & ray) const
{
	return 1.f;
}

/*float PointLight::contribution(const ComputedIntersection & info) const
{
	float d = point3f::distance(info.point, center);
	return m_intensity / (d * d);
}*/
}


