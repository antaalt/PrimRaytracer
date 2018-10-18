#pragma once

#include "Types.h"

namespace raycore {

	inline float clamp(float value, float min, float max)
	{
		if (value > max) return max;
		if (value < min) return min;
		return value;
	}

	template <typename T>
	inline Math::Vector4<T> clamp(const Math::Vector4<T> &a, float min, float max)
	{
		return Math::Vector4<T>(
			clamp(a.x, min, max),
			clamp(a.y, min, max),
			clamp(a.z, min, max),
			clamp(a.w, min, max)
		);
	}

	template <typename T>
	inline T lerp(T a, T b, float t)
	{
		return a + (b - a) * t;
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
}