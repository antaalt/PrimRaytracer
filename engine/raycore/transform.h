#pragma once
#include "Config.h"

namespace transform {

	// World coordinates have z as up
	// Screen coordinates have y as up to fit screen
	// All coordinates are left handed

	inline vec3 localToWorld(const vec3 &vec)
	{
		return vec3(vec.x, -vec.z, vec.y);
	}
	inline vec3 screenToWorld(const vec3 &vec)
	{
		return vec3(vec.x, -vec.z, vec.y);
	}
	inline point3 screenToWorld(const point3 &vec)
	{
		return point3(vec.x, -vec.z, vec.y);
	}
	inline vec3 worldToScreen(const vec3 &vec)
	{
		return vec3(vec.x, vec.z, -vec.y);
	}
	inline point3 worldToScreen(const point3 &vec)
	{
		return point3(vec.x, vec.z, -vec.y);
	}

	struct Onb {
		Onb(const norm3 &n)
		{
#if defined(SCREEN_SPACE) // screen space
			normal = vec3(n);
			if (std::fabs(n.x) > std::fabs(n.y))
				tangent = normalize(vec3(n.z, 0, -n.x));
			else
				tangent = normalize(vec3(0, -n.z, n.y));
			bitangent = normalize(cross(normal, tangent));
#else
			normal = vec3(n);
			if (math::abs(n.x) > math::abs(n.z))
				tangent = normalize(vec3(-n.y, n.x, 0.f));
			else
				tangent = normalize(vec3(0, -n.z, n.y));
			bitangent = normalize(cross(tangent, normal));
#endif
		}
		vec3 operator()(const vec3 &n)
		{
#if defined(SCREEN_SPACE) // screen space
			return vec3(n.x * bitangent + n.y * normal + n.z * tangent);
#else
			return vec3(n.x * bitangent + n.y * tangent + n.z * normal);
#endif
		}
		norm3 operator()(const norm3 &n)
		{
#if defined(SCREEN_SPACE) // screen space
			return norm3(n.x * bitangent + n.y * normal + n.z * tangent);
#else
			return norm3(n.x * bitangent + n.y * tangent + n.z * normal);
#endif
		}
	private:
		vec3 normal;
		vec3 tangent;
		vec3 bitangent;
	};

	struct PolarCoordinates {
		constexpr PolarCoordinates(math::Radian theta, float r) : theta(theta), r(r) {}
		inline PolarCoordinates(const vec2 &vec) :
			r(length(vec)),
			theta(math::arctan2(vec.y, vec.x))
		{}
		inline operator vec2()
		{
			return vec2(
				r * math::cos(theta),
				r * math::sin(theta)
			);
		}
		inline static PolarCoordinates random(float randTheta, float randRadius)
		{
			return PolarCoordinates(math::Radian(2.f * math::pi * randTheta), randRadius);
		}
	private:
		const float r;
		const math::Radian theta;
	};

	struct SphericalCoordinates {
		constexpr SphericalCoordinates(math::Radian theta, math::Radian phi, float r) : theta(theta), phi(phi), r(r) {}
		inline SphericalCoordinates(const vec3 &vec) :
			theta(math::arccos(vec.z / length(vec))),
			phi(math::arctan(vec.y / vec.x)),
			r(length(vec))
		{}
		inline operator vec3() {
			return vec3(
				r * sin(theta) * cos(phi),
				r * sin(theta) * sin(phi),
				r * cos(theta)
			);
		}
		// return Uniformly distributed random spherical coordinates on unit sphere.
		inline static SphericalCoordinates random(float randTheta, float randPhi)
		{
			return SphericalCoordinates(
				math::Radian(2.f * math::pi * randTheta),
				math::Radian(2.f * math::pi * randPhi),
				1.f
			);
		}
	private:
		const math::Radian theta, phi;
		const float r;
	};

	struct CylindricalCoordinates {
		constexpr CylindricalCoordinates(math::Radian rho, math::Radian phi, float z) : rho(rho), phi(phi), z(z) {}
		inline CylindricalCoordinates(const vec3 &vec) :
			rho(math::Radian(math::sqrt(vec.x * vec.x + vec.y * vec.y))),
			phi(
				((vec.x == 0 && vec.y == 0) ?
					0.f :
					((vec.x == 0.f) ?
						math::arcsin(vec.y / rho()) :
						((vec.x > 0.f) ?
							math::arctan(vec.y / vec.x) :
							math::arcsin(vec.y / rho())() + math::pi))
				)
			)
		{
		}
		inline operator vec3()
		{
			return vec3(
				rho() * math::cos(phi),
				rho() * math::cos(phi),
				z
			);
		}
	private:
		math::Radian rho, phi;
		float z;
	};
}