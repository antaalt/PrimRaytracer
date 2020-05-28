#pragma once

#include <cmath>
#include <climits>

namespace math {

	using real_t = float;
	using byte_t = unsigned char;

	template <typename T>
	inline T lerp(T a, T b, float t)
	{
		return a + (b - a) * t;
	}
	template <typename T>
	inline T (max)(T a, T b)
	{
		if (a > b) return a;
		return b;
	}
	template <typename T>
	inline T (min)(T a, T b)
	{
		if (a < b) return a;
		return b;
	}
	template <typename T>
	inline T clamp(T value, T min, T max)
	{
		if (value > max)
			return max;
		if (value < min)
			return min;
		return value;
	}
	template <typename T>
	inline T saturate(T value)
	{
		return clamp<T>(value, 0.f, 1.f);
	}

	// Cmath wrapper
	// http://www.cplusplus.com/reference/cmath/
	const math::real_t pi = 3.14159265358979323846f;
	/*const math::real_t maxReal = std::numeric_limits<math::real_t>::max();
	const math::real_t minReal = std::numeric_limits<math::real_t>::max();
	const math::real_t infReal = std::numeric_limits<math::real_t>::infinity();*/

	struct Radian;

	struct Degree {
		constexpr Degree() : value(0.f) {}
		constexpr Degree(math::real_t value) : value(value) {}
		inline operator Radian();
		constexpr math::real_t operator()() const { return value; }
	private:
		const math::real_t value;
	};

	struct Radian {
		constexpr Radian() : value(0.f) {}
		constexpr Radian(math::real_t value) : value(value) {}
		inline operator Degree() { return Degree(180.f * value / math::pi); }
		constexpr math::real_t operator()() const { return value; }
	private:
		const math::real_t value;
	};
	inline Degree::operator Radian() { return Radian(math::pi * value / 180.f); }

	// Trigonometric functions
	inline math::real_t cos(Radian value)
	{
		return std::cos(value());
	}
	inline math::real_t sin(Radian value)
	{
		return std::sin(value());
	}
	inline math::real_t tan(Radian value)
	{
		return std::tan(value());
	}
	inline Radian arccos(math::real_t value)
	{
		return Radian(std::acos(value));
	}
	inline Radian arcsin(math::real_t value)
	{
		return Radian(std::asin(value));
	}
	inline Radian arctan(math::real_t value)
	{
		return Radian(std::atan(value));
	}
	inline Radian arctan2(math::real_t x, math::real_t y)
	{
		return Radian(std::atan2(x, y));
	}
	// Hyperbolic functions
	// [...]
	// Exponential and logarithmic functions
	inline math::real_t exp(math::real_t value)
	{
		return std::exp(value);
	}
	inline math::real_t log(math::real_t value)
	{
		return std::log(value);
	}
	inline math::real_t log10(math::real_t value)
	{
		return std::log10(value);
	}
	// Power functions
	inline math::real_t pow(math::real_t value, int exponent)
	{
		return std::pow(value, exponent);
	}
	inline math::real_t pow(math::real_t value, math::real_t exponent)
	{
		return std::pow(value, exponent);
	}
	inline math::real_t sqrt(math::real_t value)
	{
		return std::sqrt(value);
	}
	// Rounding and remainder
	inline math::real_t ceil(math::real_t value)
	{
		return std::ceil(value);
	}
	inline math::real_t floor(math::real_t value)
	{
		return std::floor(value);
	}
	inline math::real_t trunc(math::real_t value)
	{
		return std::trunc(value);
	}
	// Others functions
	inline math::real_t abs(math::real_t value)
	{
		return (value < 0.f) ? -value : value;
	}
	// Macro
	inline math::real_t isNan(math::real_t value)
	{
		return std::isnan(value);
	}
	inline math::real_t isInf(math::real_t value)
	{
		return std::isinf(value);
	}
}