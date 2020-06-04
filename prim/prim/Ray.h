#pragma once

#include "Config.h"
#include <climits>

namespace prim {

struct Ray {
	Ray();
	Ray(point3f origin, vec3f direction, float tmin = std::numeric_limits<float>::epsilon(), float tmax = (std::numeric_limits<float>::max)());

	point3f operator()(float distance) const;

	point3f origin; // Origin of the ray
	vec3f direction; // Direction of the ray
	float tmin; // Minimal distance to avoid self intersection
	mutable float tmax; // Maximum distance. Mutable so that it can register last hit to get closest hit.
};

inline Ray::Ray() :
	tmin(std::numeric_limits<float>::epsilon()),
	tmax((std::numeric_limits<float>::max)()) 
{
}
inline Ray::Ray(geometry::point3f origin, geometry::vec3f direction, float tmin, float tmax) :
	origin(origin),
	direction(direction),
	tmin(tmin),
	tmax(tmax)
{
}

inline geometry::point3f Ray::operator()(float distance) const 
{ 
	return origin + geometry::point3f(direction * distance);
}

}