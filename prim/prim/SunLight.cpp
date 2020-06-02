#include "SunLight.h"

#include "math/geometry.h"

namespace prim {


static constexpr radianf sunAngle = radianf(0.00465472f);
static const float unitSunRadius = tan(sunAngle);

SunLight::SunLight(const norm3f &direction, const color4f &albedo, float intensity) :
	Light(albedo, intensity), 
	m_direction(direction)
{
}

bool SunLight::sample(const ComputedIntersection & info, const Scene & scene, float * pdf, vec3f * sample) const
{
	transform::Onb onb(m_direction);
	const vec3f lightSample = onb(vec3f::normalize(vec3f(unitSunRadius * sample::unitDisk(Rand::sample<float>(), Rand::sample<float>()), 1.f)));
	
	Ray shadowRay(info.point, lightSample);
	BackCulling culling;
	Intersection intersection(culling, true);
	if (scene.intersect(shadowRay, intersection))
		return false;
	*pdf = 1.f / (2.f * pi<float>() * (1.f - cos(sunAngle))); // output near 19000.f
	*sample = lightSample;
	return true;
}

float SunLight::contribution(const ComputedIntersection & info) const
{
	return shade().luminance();
}

}