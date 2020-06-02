#pragma once
#include "Config.h"

#include "math/geometry.h"

namespace transform {

// World coordinates have z as up
// Screen coordinates have y as up to fit screen
// All coordinates are left handed

inline vec3f localToWorld(const vec3f &vec)
{
	return vec3f(vec.x, -vec.z, vec.y);
}
inline vec3f screenToWorld(const vec3f &vec)
{
	return vec3f(vec.x, -vec.z, vec.y);
}
inline point3f screenToWorld(const point3f &vec)
{
	return point3f(vec.x, -vec.z, vec.y);
}
inline vec3f worldToScreen(const vec3f &vec)
{
	return vec3f(vec.x, vec.z, -vec.y);
}
inline point3f worldToScreen(const point3f &vec)
{
	return point3f(vec.x, vec.z, -vec.y);
}

struct Onb {
	Onb(const norm3f &n)
	{
#if defined(SCREEN_SPACE) // screen space
		normal = vec3f(n);
		if (std::fabs(n.x) > std::fabs(n.y))
			tangent = vec3f::normalize(vec3f(n.z, 0, -n.x));
		else
			tangent = vec3f::normalize(vec3f(0, -n.z, n.y));
		bitangent = vec3f::normalize(vec3f::cross(normal, tangent));
#else
		normal = vec3f(n);
		if (geometry::abs(n.x) > geometry::abs(n.z))
			tangent = vec3f::normalize(vec3f(-n.y, n.x, 0.f));
		else
			tangent = vec3f::normalize(vec3f(0, -n.z, n.y));
		bitangent = vec3f::normalize(vec3f::cross(tangent, normal));
#endif
	}
	vec3f operator()(const vec3f &n)
	{
#if defined(SCREEN_SPACE) // screen space
		return vec3f(n.x * bitangent + n.y * normal + n.z * tangent);
#else
		return vec3f(n.x * bitangent + n.y * tangent + n.z * normal);
#endif
	}
	norm3f operator()(const norm3f &n)
	{
#if defined(SCREEN_SPACE) // screen space
		return norm3f(n.x * bitangent + n.y * normal + n.z * tangent);
#else
		return norm3f(n.x * bitangent + n.y * tangent + n.z * normal);
#endif
	}
private:
	vec3f normal;
	vec3f tangent;
	vec3f bitangent;
};

}