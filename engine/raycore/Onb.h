#pragma once

#include "Config.h"

namespace raycore {

	namespace transform {

		struct Onb {
			Onb(const norm3 &n)
			{
				normal = n;
				if (std::fabs(n.x) > std::fabs(n.y))
					tangent = normalize(norm3(n.z, 0, -n.x));
				else
					tangent = normalize(norm3(0, -n.z, n.y));
				binormal = normalize(cross(n, tangent));
			}

			vec3 operator()(const vec3 &n)
			{
				return vec3(n.x * binormal + n.y * normal + n.z * tangent);
			}
			norm3 operator()(const norm3 &n)
			{
				return n.x * binormal + n.y * normal + n.z * tangent;
			}
		private:
			norm3 normal;
			norm3 binormal;
			norm3 tangent;
		};
	}
}