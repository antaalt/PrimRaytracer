#include "SunLight.h"

#include "../../Core/Geometry.h"

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
	float r1 = random<float>();
	float r2 = random<float>();
	Ray ray;
	Onb onb(m_direction);
	ray.origin = hitPoint;
	ray.direction = onb(vec3f::normalize(vec3f(unitSunRadius * polar<float>(2.0 * pi<float> *r2, sqrt(r1)).cartesian(), 1.f)));
	return ray;
}

float SunLight::pdf(const Ray & ray) const
{
	return 1.f / (2.f * pi<float>() * (1.f - cos(sunAngle))); // output near 19000.f
}

}