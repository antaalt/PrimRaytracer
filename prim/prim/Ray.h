#pragma once

#include "Config.h"
#include <climits>

namespace prim {

struct Culling { virtual bool operator()(float cosTheta) { return geometry::abs(cosTheta) < std::numeric_limits<float>::epsilon(); } };
struct FrontCulling : Culling { bool operator()(float cosTheta) override { return cosTheta > std::numeric_limits<float>::epsilon(); } };
struct BackCulling : Culling { bool operator()(float cosTheta) override { return cosTheta < std::numeric_limits<float>::epsilon(); } };

struct Ray {
	Ray();
	Ray(const point3f &origin, const vec3f &direction, float tmin = std::numeric_limits<float>::epsilon(), float tmax = (std::numeric_limits<float>::max)());

	point3f operator()(float distance) const;

	bool cull(float cosTheta) const;

	point3f origin; // Origin of the ray
	vec3f direction; // Direction of the ray
	Culling *culling; // Culling of the ray against geometry
	// TODO add flag terminate on first hit
	float tmin; // Minimal distance to avoid self intersection
	mutable float tmax; // Maximum distance. Mutable so that it can register last hit to get closest hit.
};

}