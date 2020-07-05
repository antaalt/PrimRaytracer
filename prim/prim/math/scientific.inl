#pragma once

#include "scientific.h"

#include <cmath>
#include <climits>

namespace geometry {

template <typename T>
inline T lerp(T a, T b, real_t t)
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

// Trigonometric functions
template <typename T>
inline T cos(radian<T> value)
{
	return std::cos(value());
}

template <typename T>
inline T sin(radian<T> value)
{
	return std::sin(value());
}

template <typename T>
inline T tan(radian<T> value)
{
	return std::tan(value());
}

template <typename T>
inline radian<T> arccos(T value)
{
	return radian<T>(std::acos(value));
}

template <typename T>
inline radian<T> arcsin(T value)
{
	return radian<T>(std::asin(value));
}

template <typename T>
inline radian<T> arctan(T value)
{
	return radian<T>(std::atan(value));
}

template <typename T>
inline radian<T> arctan2(T x, T y)
{
	return radian<T>(std::atan2(x, y));
}

// Hyperbolic functions
// [...]
// Exponential and logarithmic functions
template <typename T>
inline T exp(T value)
{
	return std::exp(value);
}

template <typename T>
inline T log(T value)
{
	return std::log(value);
}

template <typename T>
inline T log10(T value)
{
	return std::log10(value);
}

// Power functions
template <typename T>
inline T pow(T value, real_t exponent)
{
	return std::pow(value, exponent);
}

template <typename T>
inline T sqrt(T value)
{
	return static_cast<T>(std::sqrt(value));
}

// Rounding and remainder
template <typename T>
inline T ceil(T value)
{
	return std::ceil(value);
}
template <typename T>
inline T floor(T value)
{
	return std::floor(value);
}

template <typename T>
inline T trunc(T value)
{
	return std::trunc(value);
}

// Others functions
template <typename T>
inline T abs(T value)
{
	return (value < T(0)) ? -value : value;
}

// Macro
template <typename T>
inline T isNan(T value)
{
	return std::isnan(value);
}

template <typename T>
inline T isInf(T value)
{
	return std::isinf(value);
}

}