#pragma once
#include "Config.h"

#include "math/geometry.h"

namespace transform {

using namespace geometry;

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
			tangent = normalize(vec3f(n.z, 0, -n.x));
		else
			tangent = normalize(vec3f(0, -n.z, n.y));
		bitangent = normalize(cross(normal, tangent));
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

struct PolarCoordinates {
	PolarCoordinates(radianf theta, float r) : theta(theta), r(r) {}
	inline PolarCoordinates(const vec2f &vec) :
		r(vec.norm()),
		theta(geometry::arctan2(vec.y, vec.x))
	{}
	inline operator vec2f()
	{
		return vec2f(
			r * geometry::cos(theta),
			r * geometry::sin(theta)
		);
	}
	inline static PolarCoordinates random(float randTheta, float randRadius)
	{
		return PolarCoordinates(geometry::radianf(2.f * geometry::pi<float>() * randTheta), randRadius);
	}
private:
	const float r;
	const geometry::radianf theta;
};

struct SphericalCoordinates {
	SphericalCoordinates(geometry::radianf theta, geometry::radianf phi, float r) : theta(theta), phi(phi), r(r) {}
	inline SphericalCoordinates(const vec3f &vec) :
		theta(geometry::arccos(vec.z / vec.norm())),
		phi(geometry::arctan(vec.y / vec.x)),
		r(vec.norm())
	{}
	inline operator vec3f() {
		return vec3f(
			r * sin(theta) * cos(phi),
			r * sin(theta) * sin(phi),
			r * cos(theta)
		);
	}
	// return Uniformly distributed random spherical coordinates on unit sphere.
	inline static SphericalCoordinates random(float randTheta, float randPhi)
	{
		return SphericalCoordinates(
			geometry::radianf(2.f * geometry::pi<float>() * randTheta),
			geometry::radianf(2.f * geometry::pi<float>() * randPhi),
			1.f
		);
	}
private:
	const geometry::radianf theta, phi;
	const float r;
};

struct CylindricalCoordinates {
	CylindricalCoordinates(geometry::radianf rho, geometry::radianf phi, float z) : rho(rho), phi(phi), z(z) {}
	CylindricalCoordinates(const vec3f &vec) :
		rho(geometry::radianf(geometry::sqrt(vec.x * vec.x + vec.y * vec.y))),
		phi(
			((vec.x == 0 && vec.y == 0) ?
				0.f :
				((vec.x == 0.f) ?
					geometry::arcsin(vec.y / rho())() :
					((vec.x > 0.f) ?
						geometry::arctan(vec.y / vec.x)() :
						geometry::arcsin(vec.y / rho())() + geometry::pi<float>()))
			)
		)
	{
	}
	operator vec3f()
	{
		return vec3f(
			rho() * geometry::cos(phi),
			rho() * geometry::cos(phi),
			z
		);
	}
private:
	geometry::radianf rho, phi;
	float z;
};
}