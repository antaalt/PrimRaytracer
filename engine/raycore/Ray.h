#pragma once

#include "Config.h"
#include <climits>


namespace raycore {

	namespace tracer {

		enum RayType {
			GEOMETRY_RAY,
			SHADOW_RAY
		};

		struct Ray {
			geometry::point3f origin;
			geometry::vec3f direction;
			float tmin, tmax;
			RayType type;
			Ray() : tmin(EPSILON), tmax((std::numeric_limits<float>::max)()), type(GEOMETRY_RAY) {}
			Ray(geometry::point3f origin, geometry::vec3f direction, RayType ray_type = GEOMETRY_RAY, float tmin = EPSILON, float tmax = (std::numeric_limits<float>::max)()) :
				origin(origin),
				direction(direction),
				type(ray_type),
				tmin(tmin),
				tmax(tmax)
				{}

			geometry::point3f operator()(float distance) const { return origin + geometry::point3f(direction * distance); }
		};
	}
}