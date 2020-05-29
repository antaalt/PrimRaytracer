#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "uv2.h"


namespace geometry {

template <typename T>
inline vec2<T>::vec2()
{
}

template <typename T>
inline vec2<T>::vec2(T value) : x(value), y(value)
{
}

template <typename T>
inline vec2<T>::vec2(T x, T y) : x(x), y(y)
{
}

template<typename T>
template<typename U>
inline vec2<T>::vec2(const vec2<U>& vec) : x(static_cast<T>(vec.x)), y(static_cast<T>(vec.y))
{
}

template<typename T>
inline vec2<T>::vec2(const uv2<T>& uv) : x(uv.u), y(uv.v)
{
}

template <typename T>
inline vec2<T>::vec2(const vec3<T> &vec) : x(vec.x), y(vec.y)
{
}

template<typename T>
inline vec2<T>::vec2(const vec4<T>& vec) : x(vec.x), y(vec.y)
{
}

template <typename T>
inline T &vec2<T>::operator[](size_t index)
{
	return data[index];
}

template <typename T>
inline const T &vec2<T>::operator[](size_t index) const
{
	return data[index];
}

template <typename T>
inline T vec2<T>::norm() const
{
	return sqrt(dot(*this, *this));
}

template <typename T>
inline T vec2<T>::dot(const vec2 &lhs, const vec2 &rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

template <typename T>
inline bool operator==(const vec2<T> & lhs, const vec2<T> & rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <typename T>
inline bool operator!=(const vec2<T> &lhs, const vec2<T> &rhs)
{
	return !(lhs == rhs);
}

template <typename T>
inline vec2<T> operator*(const vec2<T> &lhs, T rhs)
{
	vec2<T> out(lhs);
	out *= rhs;
	return out;
}

template <typename T>
inline vec2<T> operator*(T lhs, const vec2<T> &rhs)
{
	vec2<T> out(rhs);
	out *= lhs;
	return out;
}

template<typename T>
vec2<T> operator*(const vec2<T>& lhs, const vec2<T>& rhs)
{
	vec2<T> out(lhs);
	out *= rhs;
	return out;
}

template <typename T>
inline vec2<T> & operator*=(vec2<T> & lhs, T rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

template<typename T>
vec2<T>& operator*=(vec2<T>& lhs, const vec2<T>& rhs)
{
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}

template <typename T>
inline vec2<T> operator/(const vec2<T> &lhs, T rhs)
{
	vec2<T> out(lhs);
	out /= rhs;
	return out;
}

template<typename T>
vec2<T> operator/(const vec2<T>& lhs, const vec2<T>& rhs)
{
	vec2<T> out(lhs);
	out /= rhs;
	return out;
}

template <typename T>
inline vec2<T> & operator/=(vec2<T> & lhs, T rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
	return lhs;
}

template<typename T>
vec2<T>& operator/=(vec2<T>& lhs, const vec2<T>& rhs)
{
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	return lhs;
}

template <typename T>
inline vec2<T> operator+(const vec2<T> &lhs, const vec2<T> &rhs)
{
	vec2<T> out(lhs);
	out += rhs;
	return out;
}

template <typename T>
inline vec2<T> & operator+=(vec2<T> & lhs, const vec2<T> & rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

template <typename T>
inline vec2<T> operator-(const vec2<T> &lhs, const vec2<T> &rhs)
{
	vec2<T> out(lhs);
	out -= rhs;
	return out;
}

template <typename T>
inline vec2<T> & operator-=(vec2<T> & lhs, const vec2<T> & rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

template <typename T>
inline vec2<T> operator-(const vec2<T> &vec)
{
	return vec2<T>(-vec.x, -vec.y);
}

}