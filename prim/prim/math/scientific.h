#pragma once

#include <climits>

#include "angle.h"

namespace geometry {

using real_t = float;

template <typename T> T lerp(T a, T b, real_t t);
template <typename T> T max(T a, T b);
template <typename T> T min(T a, T b);
template <typename T> T clamp(T value, T min, T max);

// Cmath wrapper
// http://www.cplusplus.com/reference/cmath/

// Trigonometric functions
template <typename T> T cos(radian<T> value);
template <typename T> T sin(radian<T> value);
template <typename T> T tan(radian<T> value);
template <typename T> radian<T> arccos(T value);
template <typename T> radian<T> arcsin(T value);
template <typename T> radian<T> arctan(T value);
template <typename T> radian<T> arctan2(T x, T y);
// Hyperbolic functions
// [...]
// Exponential and logarithmic functions
template <typename T> T exp(T value);
template <typename T> T log(T value);
template <typename T> T log10(T value);
// Power functions
template <typename T> T pow(T value, real_t exponent);
template <typename T> T sqrt(T value);
// Rounding and remainder
template <typename T> T ceil(T value);
template <typename T> T floor(T value);
template <typename T> T trunc(T value);
// Others functions
template <typename T> T abs(T value);
// Macro
template <typename T> T isNan(T value);
template <typename T> T isInf(T value);

}