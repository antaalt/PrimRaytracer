#include "Ray.h"

namespace prim {

Ray::Ray() :
	culling(nullptr),
	tmin(std::numeric_limits<float>::epsilon()),
	tmax((std::numeric_limits<float>::max)())
{
}
Ray::Ray(const point3f &origin, const vec3f &direction, float tmin, float tmax) :
	origin(origin),
	direction(direction),
	culling(nullptr),
	tmin(tmin),
	tmax(tmax)
{
}

point3f Ray::operator()(float distance) const
{
	return origin + direction * distance;
}

bool Ray::cull(float cosTheta) const
{
	if (culling != nullptr)
		return (*culling)(cosTheta);
	return false;
}

}