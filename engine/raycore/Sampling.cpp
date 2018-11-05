#include "Sampling.h"



namespace raycore {
	namespace sample {
		vec3 unitSphere(float r1, float r2)
		{
			float phi = 2.f * M_PIf * r1;
			float theta = acosf(1.f - 2.f * r2);
			return vec3(
				2.f * cosf(2.f * M_PIf * r1) * sqrtf(r2 * (1.f - r2)),
				2.f * sinf(2.f * M_PIf * r1) * sqrtf(r2 * (1.f - r2)),
				1.f - 2.f * r2
			);
		}
		vec3 unitHemisphere(float r1, float r2)
		{
			const float phi = 2.f * M_PIf * r1;
			const float r = r2;
			return vec3(
				r * cosf(phi),
				sqrtf(1.f - r * r),
				r * sinf(phi)
			);
		}
		vec3 unitMicrofacet(float roughness, float r1, float r2)
		{
			float phi = std::atanf(roughness * std::sqrtf(r1) / std::sqrtf(1 - r1));
			float theta = 2 * M_PIf * r2;
			return vec3(
				std::sinf(phi) * std::cosf(theta),
				std::cosf(phi),
				std::sinf(phi) * std::sinf(theta)
			);
		}
		vec3 unitDisk(float r1, float r2)
		{
			float r = sqrtf(r1);
			float phi = 2.f * M_PIf * r2;
			return vec3(
				r * cosf(phi),
				r * sinf(phi),
				0.f
			);
		}
	}
}