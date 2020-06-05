#include "color4.h"

namespace geometry {

template <typename T>
inline color4<T>::color4()
{
}

template <typename T>
inline color4<T>::color4(T value) :
	r(value), g(value), b(value), a(value)
{
}

template<typename T>
template<typename U>
inline color4<T>::color4(const color4<U>& value) :
	color4(static_cast<T>(value.r), static_cast<T>(value.g), static_cast<T>(value.b), static_cast<T>(value.a))
{
}

template <typename T>
inline T & color4<T>::operator[](size_t index)
{
	return data[index];
}

template <typename T>
inline const T & color4<T>::operator[](size_t index) const
{
	return data[index];
}

template<typename T>
inline T color4<T>::luminance() const
{
	static const vec3f l(0.2126f, 0.7152f, 0.0722f);
	return r * l.x + g * l.y + b * l.z;
}

template<typename T>
inline T srgb2linear(T value)
{
	// TODO assert float
	if (value <= T(0.04045))
		return value * T(1) / T(12.92);
	return pow<float>((value + T(0.055)) * T(1) / T(1.055), T(2.4));
}

template<typename T>
inline T linear2srgb(T value)
{
	// TODO assert float
	if (value <= T(0.0031308))
		return T(12.92) * value;
	return T(1.055) * pow<float>(value, T(1) / T(2.4)) - T(0.055);
}

template<typename T>
inline color4<T> color4<T>::srgb2linear(const color4<T>& color)
{
	return color4<T>(
		srgb2linear(color.r),
		srgb2linear(color.g),
		srgb2linear(color.b),
		color.a
	);
}

template<typename T>
inline color4<T> color4<T>::linear2srgb(const color4<T>& color)
{
	return color4<T>(
		linear2srgb(color.r),
		linear2srgb(color.g),
		linear2srgb(color.b),
		color.a
	);
}

template <typename T>
inline color4<T>::color4(T r, T g, T b, T a) :
	r(r), g(g), b(b), a(a)
{
}

template <typename T>
inline bool operator==(const color4<T> &lhs, const color4<T> &rhs)
{
	return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
}

template <typename T>
inline bool operator!=(const color4<T> &lhs, const color4<T> &rhs)
{
	return !(lhs == rhs);
}

template <typename T>
inline color4<T> operator*(const color4<T> &lhs, const color4<T> &rhs)
{
	color4<T> out(lhs);
	out *= rhs;
	return out;
}

template <typename T>
inline color4<T> operator*(const color4<T> &lhs, real_t rhs)
{
	color4<T> out(lhs);
	out *= rhs;
	return out;
}

template <typename T>
inline color4<T> operator*(T lhs, const color4<T> &rhs)
{
	color4<T> out(rhs);
	out *= lhs;
	return out;
}

template <typename T>
color4<T> & operator*=(color4<T> & lhs, const color4<T> & rhs)
{
	lhs.r *= rhs.r;
	lhs.g *= rhs.g;
	lhs.b *= rhs.b;
	lhs.a *= rhs.a;
	return lhs;
}

template <typename T>
color4<T> & operator*=(color4<T> & lhs, real_t rhs)
{
	lhs.r = static_cast<T>(lhs.r * rhs);
	lhs.g = static_cast<T>(lhs.g * rhs);
	lhs.b = static_cast<T>(lhs.b * rhs);
	lhs.a = static_cast<T>(lhs.a * rhs);
	return lhs;
}

template <typename T>
inline color4<T> operator/(const color4<T> &lhs, real_t rhs)
{
	return color4<T>(lhs.r / rhs, lhs.g / rhs, lhs.b / rhs, lhs.a / rhs);
}

template <typename T>
color4<T> & operator/=(color4<T> & lhs, real_t rhs)
{
	lhs.r = static_cast<T>(lhs.r / rhs);
	lhs.g = static_cast<T>(lhs.g / rhs);
	lhs.b = static_cast<T>(lhs.b / rhs);
	lhs.a = static_cast<T>(lhs.a / rhs);
	return lhs;
}

template <typename T>
inline color4<T> operator+(const color4<T> &lhs, const color4<T> &rhs)
{
	return color4<T>(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a);
}

template <typename T>
inline color4<T> &operator+=(color4<T> &lhs, const color4<T> &rhs)
{
	lhs.r += rhs.r; lhs.g += rhs.g; lhs.b += rhs.b; lhs.a += rhs.a; return lhs;
}

template <typename T>
inline color4<T> operator-(const color4<T> &lhs, const color4<T> &rhs)
{
	return color4<T>(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a);
}

template <typename T>
inline color4<T> &operator-=(color4<T> &lhs, const color4<T> &rhs)
{
	lhs.r -= rhs.r; lhs.g -= rhs.g; lhs.b -= rhs.b; lhs.a -= rhs.a; return lhs;
}

template <typename T>
inline color4<T> operator-(const color4<T> &vec)
{
	return color4<T>(-vec.r, -vec.g, -vec.b, -vec.a);
}

}