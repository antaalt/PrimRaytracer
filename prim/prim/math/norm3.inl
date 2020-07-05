#include "point3.h"
#include "vec3.h"
#include "norm3.h"

namespace geometry {

template <typename T>
inline norm3<T>::norm3()
{
}

template <typename T>
inline norm3<T>::norm3(T value) :
	x(value), y(value), z(value)
{
}

template <typename T>
inline norm3<T>::norm3(T x, T y, T z) :
	x(x), y(y), z(z)
{
}

template <typename T>
inline norm3<T>::norm3(const vec3<T> & vec) :
	x(vec.x), y(vec.y), z(vec.z)
{
}

template<typename T>
inline norm3<T>::norm3(const col3<T>& col) :
	norm3(col.x, col.y, col.z)
{
}

template<typename T>
inline norm3<T>::norm3(const col4<T>& col) :
	norm3(col.x, col.y, col.z)
{
}

template <typename T>
inline norm3<T>::norm3(const point3<T> & point) : 
	x(point.x), y(point.y), z(point.z)
{
}

template <typename T>
inline T & norm3<T>::operator[](size_t index)
{
	return data[index];
}

template <typename T>
inline const T & norm3<T>::operator[](size_t index) const
{
	return data[index];
}

template <typename T>
inline T norm3<T>::norm() const
{
	return sqrt(dot(*this, *this));
}

template <typename T>
inline T norm3<T>::dot(const norm3<T> &lhs, const norm3<T> &rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template <typename T>
inline norm3<T> norm3<T>::normalize(const norm3<T> &vec)
{
	return vec / vec.norm();
}

template <typename T>
inline norm3<T> norm3<T>::faceForward(const norm3<T> &n, const vec3<T> &v)
{
	return (vec3<T>::dot(v, vec3<T>(n)) < 0.f) ? -n : n;
}

template <typename T>
inline bool operator==(const norm3<T> &lhs, const norm3<T> &rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template <typename T>
inline bool operator!=(const norm3<T> &lhs, const norm3<T> &rhs)
{
	return !(lhs == rhs);
}

template <typename T>
inline norm3<T> operator*(const norm3<T> &lhs, float rhs)
{
	norm3<T> out(lhs);
	out *= rhs;
	return out;
}

template <typename T>
inline norm3<T> operator*(float lhs, const norm3<T> &rhs)
{
	norm3<T> out(rhs);
	out *= lhs;
	return out;
}

template <typename T>
inline norm3<T> & operator*=(norm3<T> & lhs, float rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	return lhs;
}

template <typename T>
inline norm3<T> operator/(const norm3<T> &lhs, float rhs)
{
	norm3<T> out(lhs);
	out /= rhs;
	return out;
}

template <typename T>
inline norm3<T> & operator/=(norm3<T> & lhs, float rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;
	return lhs;
}

template <typename T>
inline norm3<T> operator+(const norm3<T> &lhs, const norm3<T> &rhs)
{
	norm3<T> out(lhs);
	out += rhs;
	return out;
}

template <typename T>
inline norm3<T> & operator+=(norm3<T> & lhs, const norm3<T> &rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}

template <typename T>
inline norm3<T> operator-(const norm3<T> &lhs, const norm3<T> &rhs)
{
	norm3<T> out(lhs);
	out -= rhs;
	return out;
}

template <typename T>
inline norm3<T> & operator-=(norm3<T> & lhs, const norm3<T> &rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

template <typename T>
inline norm3<T> operator-(const norm3<T> &vec)
{
	return norm3<T>(-vec.x, -vec.y, -vec.z);
}

}