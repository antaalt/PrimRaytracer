#include "ShapeDisk.h"

#include "../Core/Random.h"
#include "../Core/Transform.h"

namespace prim {

ShapeDisk::ShapeDisk(const point3f & center, float radius, const norm3f &up) : m_center(center), m_radius(radius), m_up(up)
{
}

vec3f ShapeDisk::sample(const point3f & from) const
{
	vec3f sample = vec3f(sample::unitDisk(Rand::sample<float>(), Rand::sample<float>()), 1.f);
	Onb onb(m_up);
	return vec3f(m_center) + onb(sample * m_radius);
}

float ShapeDisk::pdf() const
{
	return geometry::pi<float>() * m_radius * m_radius;
}

point3f ShapeDisk::position() const
{
	return m_center;
}

}