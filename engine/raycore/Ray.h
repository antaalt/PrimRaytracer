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
			point3 origin;
			vec3 direction;
			float tmin, tmax;
			RayType type;
			Ray() : tmin(EPSILON), tmax((std::numeric_limits<float>::max)()), type(GEOMETRY_RAY) {}
			Ray(point3 origin, vec3 direction, RayType ray_type = GEOMETRY_RAY, float tmin = EPSILON, float tmax = (std::numeric_limits<float>::max)()) :
				origin(origin),
				direction(direction),
				type(ray_type),
				tmin(tmin),
				tmax(tmax)
				{}

			point3 operator()(float distance) const { return point3(origin + direction * distance); }
		};
	}
}