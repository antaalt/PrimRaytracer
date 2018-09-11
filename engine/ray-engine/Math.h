#pragma once

#include "Types.h"

namespace app {

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
	template <typename T>
	inline T max(T a, T b)
	{
		if (a > b) return a;
		return b;
	}
	template <typename T>
	inline T min(T a, T b)
	{
		if (a < b) return a;
		return b;
	}

	namespace physics {

		// Return the reflectance of a material given its eta
		// @see http://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/reflection_refraction.pdf
		inline float fresnel(float cos, float eta)
		{
			float R0 = pow(eta - 1.f, 2) / pow(eta + 1.f, 2);
			return R0 + (1.f - R0) * pow(1 - cos, 5);
		}

		inline float fresnel_schlick(const float cos_theta, const float exponent = 5.0f, const float minimum = 0.0f, const float maximum = 1.0f)
		{
			return clamp(minimum + (maximum - minimum) * powf(fmaxf(0.0f, 1.0f - cos_theta), exponent), minimum, maximum);
		}
	}
}