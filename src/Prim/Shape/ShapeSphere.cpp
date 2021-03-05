#include "ShapeSphere.h"

namespace prim {

ShapeSphere::ShapeSphere(const point3f & center, float radius) : m_center(center), m_radius(radius)
{
}
vec3f ShapeSphere::sample(const point3f & from) const
{
	float r1 = random<float>();
	float r2 = random<float>();
	vec3f sample = spherical<float>(2.f * pi<float> *r1, arccos(1.f - 2.f * r2), 1.f).cartesian();
	return (m_center - from) + sample * m_radius;
}
float ShapeSphere::pdf() const
{
	return 4.f * pi<float>() * m_radius * m_radius;;
}
point3f ShapeSphere::position() const
{
	return m_center;
}

}