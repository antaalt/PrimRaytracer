#pragma once

#include "Types.h"

namespace raycore {

	namespace transform {

		struct Onb {
			Onb(const norm3 &n)
			{
				normal = n;
				if (std::fabs(n.x) > std::fabs(n.y))
					tangent = vec3::normalize(vec3(n.z, 0, -n.x));
				else
					tangent = vec3::normalize(vec3(0, -n.z, n.y));
				binormal = vec3::cross(n, tangent);
			}

			vec3 operator()(const norm3 &n)
			{
				return n.x * binormal + n.y * normal + n.z * tangent;
			}
		private:
			vec3 normal;
			vec3 binormal;
			vec3 tangent;
		};
	}
}