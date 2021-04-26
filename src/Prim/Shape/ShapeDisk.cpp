#include "ShapeDisk.h"

#include "../Core/Transform.h"

namespace prim {

ShapeDisk::ShapeDisk(const point3f & center, float radius, const norm3f &up) : m_center(center), m_radius(radius), m_up(up)
{
}

vec3f ShapeDisk::sample(const point3f & from) const
{
	float r1 = random<float>();
	float r2 = random<float>();
	vec3f sample = vec3f(polar<float>(2.0 * pi<float> *r2, sqrt(r1)).cartesian(), 1.f);
	Onb onb(m_up);
	return vec3f(m_center) + onb(sample * m_radius);
}

float ShapeDisk::pdf() const
{
	return pi<float>.radian() * m_radius * m_radius;
}

point3f ShapeDisk::position() const
{
	return m_center;
}

}