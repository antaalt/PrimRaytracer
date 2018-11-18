#include "Sampling.h"
#include "transform.h"


namespace raycore {
	namespace sample {
		vec3 unitSphere(float r1, float r2)
		{
			return transform::SphericalCoordinates(
				math::Radian(2.f * math::pi * r1), 
				math::arccos(1.f - 2.f * r2),
				1.f
			);
		}
		vec3 unitHemisphere(float r1, float r2)
		{
			return vec3(
				transform::PolarCoordinates(
					math::Radian(2.f * math::pi * r1),
					r2
				), 
				math::sqrt(1.f - r2 * r2)
			);
		}
		vec3 unitMicrofacet(float roughness, float r1, float r2)
		{
			return transform::SphericalCoordinates(
				math::arctan(roughness * math::sqrt(r1) / math::sqrt(1.f - r1)),
				math::Radian(2.f * math::pi * r2),
				1.f
			);
		}
		vec2 unitDisk(float r1, float r2)
		{
			return transform::PolarCoordinates(
				math::Radian(2.f * math::pi * r2),
				math::sqrt(r1)
			);
		}
	}
}