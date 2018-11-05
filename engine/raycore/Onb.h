#pragma once

#include "Config.h"

namespace raycore {

	namespace transform {

		struct Onb {
			Onb(const norm3 &n)
			{
				normal = vec3(n);
				if (std::fabs(n.x) > std::fabs(n.y))
					tangent = normalize(vec3(n.z, 0, -n.x));
				else
					tangent = normalize(vec3(0, -n.z, n.y));
				binormal = normalize(cross(normal, tangent));
			}

			vec3 operator()(const vec3 &n)
			{
				return vec3(n.x * binormal + n.y * normal + n.z * tangent);
			}
			norm3 operator()(const norm3 &n)
			{
				return norm3(n.x * binormal + n.y * normal + n.z * tangent);
			}
		private:
			vec3 normal;
			vec3 binormal;
			vec3 tangent;
		};
	}
}