#include "SunLight.h"

#include "../../Core/Random.h"

namespace prim {

static constexpr radianf sunAngle = radianf(0.00465472f);
static const float unitSunRadius = tan(sunAngle);

SunLight::SunLight(const norm3f &direction, const color4f &albedo, float intensity) :
	Light(albedo, intensity), 
	m_direction(direction)
{
}

Ray SunLight::sample(const point3f &hitPoint) const
{
	Ray ray;
	Onb onb(m_direction);
	ray.origin = hitPoint;
	ray.direction = onb(vec3f::normalize(vec3f(unitSunRadius * sample::unitDisk(Rand::sample<float>(), Rand::sample<float>()), 1.f)));
	return ray;
}

float SunLight::pdf(const Ray & ray) const
{
	return 1.f / (2.f * pi<float>() * (1.f - cos(sunAngle))); // output near 19000.f
}

}