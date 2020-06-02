#include "ShapeSphere.h"

#include "Random.h"

namespace prim {

ShapeSphere::ShapeSphere(const point3f & center, float radius) : m_center(center), m_radius(radius)
{
}
vec3f ShapeSphere::sample(const point3f & from) const
{
	vec3f sample = sample::unitSphere(Rand::sample<float>(), Rand::sample<float>());
	return vec3f(m_center - from) + sample * m_radius;
}
float ShapeSphere::pdf() const
{
	return 4.f * geometry::pi<float>() * m_radius * m_radius;;
}
point3f ShapeSphere::position() const
{
	return m_center;
}

}