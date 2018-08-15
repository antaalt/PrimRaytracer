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

	inline float clamp(float value, float min, float max)
	{
		if (value > max) return max;
		if (value < min) return min;
		return value;
	}

	inline float lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	inline color4 lerp(const color4 &a, const color4 &b, float t)
	{
		return color4(
			lerp(a.x, b.x, t),
			lerp(a.y, b.y, t),
			lerp(a.z, b.z, t),
			lerp(a.w, b.w, t)
		);
	}
}