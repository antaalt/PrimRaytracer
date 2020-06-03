#include "Transform.h"

namespace prim {

Onb::Onb(const norm3f & n)
{
#if defined(SCREEN_SPACE) // screen space
	m_normal = vec3f(n);
	if (std::fabs(n.x) > std::fabs(n.y))
		m_tangent = vec3f::normalize(vec3f(n.z, 0, -n.x));
	else
		m_tangent = vec3f::normalize(vec3f(0, -n.z, n.y));
	m_bitangent = vec3f::normalize(vec3f::cross(m_normal, m_tangent));
#else
	m_normal = vec3f(n);
	if (geometry::abs(n.x) > geometry::abs(n.z))
		m_tangent = vec3f::normalize(vec3f(-n.y, n.x, 0.f));
	else
		m_tangent = vec3f::normalize(vec3f(0, -n.z, n.y));
	m_bitangent = vec3f::normalize(vec3f::cross(m_tangent, m_normal));
#endif
}

vec3f Onb::operator()(const vec3f & n)
{
#if defined(SCREEN_SPACE) // screen space
	return vec3f(n.x * m_bitangent + n.y * m_normal + n.z * m_tangent);
#else
	return vec3f(n.x * m_bitangent + n.y * m_tangent + n.z * m_normal);
#endif
}

norm3f Onb::operator()(const norm3f & n)

{
#if defined(SCREEN_SPACE) // screen space
	return norm3f(n.x * m_bitangent + n.y * m_normal + n.z * m_tangent);
#else
	return norm3f(n.x * m_bitangent + n.y * m_tangent + n.z * m_normal);
#endif
}

BoundingBox Transform::operator()(const BoundingBox &bbox) const
{
	BoundingBox out;
	out.min = (*this)(bbox.min);
	out.max = (*this)(bbox.max);
	return out;
}

}