#pragma once

#include "Types.h"

namespace app {

	namespace transform {

		struct Onb {
			Onb(const norm3 &n)
			{
				normal = n;
				if (std::fabs(n.x) > std::fabs(n.z))
					binormal = vec3::normalize(vec3(-n.y, n.x, 0.f));
				else
					binormal = vec3::normalize(vec3(0, -n.z, n.y));
				tangent = vec3::cross(binormal, normal);
			}

			inline vec3 transform(const norm3 &p)
			{
				return p.x * tangent + p.y * binormal + p.z * normal;
			}

			vec3 normal;
			vec3 binormal;
			vec3 tangent;
		};
	}
}