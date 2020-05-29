#include "vec3.h"
#include "norm3.h"
#include "point3.h"
#include "vec2.h"
#include "vec4.h"

namespace geometry {

template <typename T>
inline vec3<T>::vec3()
{
}

template <typename T>
inline vec3<T>::vec3(T value) :
	x(value), y(value), z(value)
{
}

template <typename T>
inline vec3<T>::vec3(T x, T y, T z) : 
	x(x), y(y), z(z)
{
}

template <typename T>
inline vec3<T>::vec3(const norm3<T> &normal) : 
	x(normal.x), y(normal.y), z(normal.z)
{
}

template <typename T>
inline vec3<T>::vec3(const point3<T> &point) : 
	x(point.x), y(point.y), z(point.z)
{
}

template<typename T>
inline vec3<T>::vec3(const col3<T>& col) :
	x(col.x), y(col.y), z(col.z)
{
}

template<typename T>
inline vec3<T>::vec3(const col4<T>& col) :
	vec3(col.x, col.y, col.z)
{
}

template <typename T>
inline vec3<T>::vec3(const vec2<T> &vec, T z) : 
	x(vec.x), y(vec.y), z(z)
{
}

template<typename T>
inline vec3<T>::vec3(const vec4<T>& vec) :
	x(vec.x), y(vec.y), z(vec.z)
{
}

template <typename T>
inline T & vec3<T>::operator[](size_t index)
{
	return data[index];
}

template <typename T>
inline const T & vec3<T>::operator[](size_t index) const
{
	return data[index];
}

template <typename T>
inline T vec3<T>::norm() const
{
	return sqrt(dot(*this, *this));
}

template <typename T>
inline T vec3<T>::dot(const vec3<T> & lhs, const vec3<T> & rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template <typename T>
inline vec3<T> vec3<T>::cross(const vec3<T> & lhs, const vec3<T> & rhs)
{
	return vec3<T>(
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x
	);
}

template <typename T>
inline vec3<T> vec3<T>::normalize(const vec3<T> &vec)
{
	return vec / vec.norm();
}

template <typename T>
inline bool operator==(const vec3<T> & lhs, const vec3<T> & rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template <typename T>
inline bool operator!=(const vec3<T> &lhs, const vec3<T> &rhs)
{
	return !(lhs == rhs);
}

template <typename T>
inline vec3<T> operator*(const vec3<T> &lhs, T rhs)
{
	vec3<T> out(lhs);
	out *= rhs;
	return out;
}

template <typename T>
inline vec3<T> operator*(T lhs, const vec3<T> &rhs)
{
	vec3<T> out(rhs);
	out *= lhs;
	return out;
}

template <typename T>
inline vec3<T> & operator*=(vec3<T> & lhs, T rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	return lhs;
}

template <typename T>
inline vec3<T> operator/(const vec3<T> &lhs, T rhs)
{
	vec3<T> out(lhs);
	out /= rhs;
	return out;
}

template <typename T>
inline vec3<T> & operator/=(vec3<T> & lhs, T rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;
	return lhs;
}

template <typename T>
inline vec3<T> operator+(const vec3<T> &lhs, const vec3<T> &rhs)
{
	return vec3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

template <typename T>
inline vec3<T> & operator+=(vec3<T> & lhs, const vec3<T> & rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}

template <typename T>
inline vec3<T> operator-(const vec3<T> &lhs, const vec3<T> &rhs)
{
	vec3<T> out(lhs);
	out -= rhs;
	return out;
}

template <typename T>
inline vec3<T> & operator-=(vec3<T> & lhs, const vec3<T> & rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

template <typename T>
inline vec3<T> operator-(const vec3<T> &vec)
{
	return vec3<T>(-vec.x, -vec.y, -vec.z);
}

}