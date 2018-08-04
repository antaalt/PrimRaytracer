#pragma once

#include "Types.h"

namespace app {

	namespace physics {

		// Return the reflectance of a material given its eta
		// @see http://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/reflection_refraction.pdf
		inline float fresnel(float cos, float eta)
		{
			float R0 = pow(eta - 1.f, 2) / pow(eta + 1.f, 2);
			return R0 + (1.f - R0) * pow(1 - cos, 5);
		}
	}

	inline float lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	inline ColorHDR lerp(const ColorHDR &a, const ColorHDR &b, float t)
	{
		return ColorHDR(
			lerp(a.x, b.x, t),
			lerp(a.y, b.y, t),
			lerp(a.z, b.z, t),
			lerp(a.w, b.w, t)
		);
	}
}