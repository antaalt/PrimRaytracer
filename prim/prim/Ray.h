#pragma once

#include "Config.h"
#include <climits>

namespace prim {

struct Ray {
	Ray();
	Ray(geometry::point3f origin, geometry::vec3f direction, float tmin = std::numeric_limits<float>::epsilon(), float tmax = (std::numeric_limits<float>::max)());

	geometry::point3f operator()(float distance) const;

	geometry::point3f origin;
	geometry::vec3f direction;
	float tmin, tmax;
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