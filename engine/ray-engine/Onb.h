#pragma once

#include "Types.h"

namespace Application::RayTracer {

	struct Onb {
		Onb(const Normal &n)
		{
			normal = n;
			if (std::fabs(n.x) > std::fabs(n.z))
				binormal = Vector3::normalize(Vector3(-n.y, n.x, 0.f));
			else
				binormal = Vector3::normalize(Vector3(0, -n.z, n.y));
			tangent = Vector3::cross(binormal, normal);
		}

		inline Vector3 transform(const Normal &p)
		{
			return p.x * tangent + p.y * binormal + p.z * normal;
		}

		Vector3 normal;
		Vector3 binormal;
		Vector3 tangent;
	};
}