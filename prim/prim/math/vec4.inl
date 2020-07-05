#include "vec4.h"
#include "norm3.h"
#include "point3.h"
#include "vec2.h"

namespace geometry {

template <typename T>
inline vec4<T>::vec4()
{
}

template <typename T>
inline vec4<T>::vec4(T value) :
	x(value), y(value), z(value)
{
}

template <typename T>
inline vec4<T>::vec4(T x, T y, T z, T w) : 
	x(x), y(y), z(z), w(w)
{
}

template <typename T>
inline vec4<T>::vec4(const vec2<T> &vec, T z, T w) :
	x(vec.x), y(vec.y), z(z), w(w)
{
}

template <typename T>
inline vec4<T>::vec4(const vec3<T> &vec, T w) :
	x(vec.x), y(vec.y), z(vec.z), w(w)
{
}

template<typename T>
inline vec4<T>::vec4(const col4<T>& vec) :
	x(vec.x), y(vec.y), z(vec.z), w(vec.w)
{
}

template <typename T>
inline T & vec4<T>::operator[](size_t index)
{
	return data[index];
}

template <typename T>
inline const T & vec4<T>::operator[](size_t index) const
{
	return data[index];
}

template <typename T>
inline T vec4<T>::norm() const
{
	return sqrt(dot(*this, *this));
}

template <typename T>
inline T vec4<T>::dot(const vec4<T> & lhs, const vec4<T> & rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

template <typename T>
inline vec4<T> vec4<T>::normalize(const vec4<T> &vec)
{
	return vec / vec.norm();
}

template <typename T>
inline bool operator==(const vec4<T> & lhs, const vec4<T> & rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

template <typename T>
inline bool operator!=(const vec4<T> &lhs, const vec4<T> &rhs)
{
	return !(lhs == rhs);
}

template <typename T>
inline vec4<T> operator*(const vec4<T> &lhs, T rhs)
{
	vec4<T> out(lhs);
	out *= rhs;
	return out;
}

template <typename T>
inline vec4<T> operator*(T lhs, const vec4<T> &rhs)
{
	vec4<T> out(rhs);
	out *= lhs;
	return out;
}

template <typename T>
inline vec4<T> & operator*=(vec4<T> & lhs, T rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	lhs.w *= rhs;
	return lhs;
}

template <typename T>
inline vec4<T> operator/(const vec4<T> &lhs, T rhs)
{
	vec4<T> out(lhs);
	out /= rhs;
	return out;
}

template <typename T>
inline vec4<T> & operator/=(vec4<T> & lhs, T rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;
	lhs.w /= rhs;
	return lhs;
}

template <typename T>
inline vec4<T> operator+(const vec4<T> &lhs, const vec4<T> &rhs)
{
	return vec4<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

template <typename T>
inline vec4<T> & operator+=(vec4<T> & lhs, const vec4<T> & rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	lhs.w += rhs.w;
	return lhs;
}

template <typename T>
inline vec4<T> operator-(const vec4<T> &lhs, const vec4<T> &rhs)
{
	vec4<T> out(lhs);
	out -= rhs;
	return out;
}

template <typename T>
inline vec4<T> & operator-=(vec4<T> & lhs, const vec4<T> & rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	lhs.w -= rhs.w;
	return lhs;
}

template <typename T>
inline vec4<T> operator-(const vec4<T> &vec)
{
	return vec4<T>(-vec.x, -vec.y, -vec.z, -vec.w);
}

}