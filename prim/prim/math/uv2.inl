#include "uv2.h"
#include "vec2.h"

namespace geometry {

template <typename T>
inline uv2<T>::uv2()
{
}

template <typename T>
inline uv2<T>::uv2(T value) : u(value), v(value)
{
}

template <typename T>
inline uv2<T>::uv2(T u, T v) : u(u), v(v)
{
}

template<typename T>
inline uv2<T>::uv2(const vec2<T>& vec) : u(vec.x), v(vec.y)
{
}

template <typename T>
inline T & uv2<T>::operator[](size_t index)
{
	return data[index];
}

template <typename T>
inline const T & uv2<T>::operator[](size_t index) const
{
	return data[index];
}

template <typename T>
inline bool operator==(const uv2<T> &lhs, const uv2<T> &rhs)
{
	return lhs.u == rhs.u && lhs.v == rhs.v;
}

template <typename T>
inline bool operator!=(const uv2<T> &lhs, const uv2<T> &rhs)
{
	return !(lhs == rhs);
}

template <typename T>
inline uv2<T> operator*(const uv2<T> &lhs, T rhs)
{
	uv2<T> out(lhs);
	out *= rhs;
	return out;
}

template <typename T>
inline uv2<T> operator*(T lhs, const uv2<T> &rhs)
{
	uv2<T> out(rhs);
	out *= lhs;
	return out;
}

template<typename T>
uv2<T> operator*=(uv2<T>& lhs, T rhs)
{
	lhs.u *= rhs;
	lhs.v *= rhs;
	return lhs;
}

template <typename T>
inline uv2<T> operator/(const uv2<T> &lhs, T rhs)
{
	uv2<T> out(lhs);
	out /= rhs;
	return out;
}

template<typename T>
uv2<T>& operator/=(uv2<T>& lhs, T rhs)
{
	lhs.u /= rhs;
	lhs.v /= rhs;
	return lhs;
}

template <typename T>
inline uv2<T> operator+(const uv2<T> &lhs, const uv2<T> &rhs)
{
	uv2<T> out(lhs);
	out += rhs;
	return out;
}

template<typename T>
uv2<T>& operator+=(uv2<T>& lhs, const uv2<T>& rhs)
{
	lhs.u += rhs.u;
	lhs.v += rhs.v;
	return lhs;
}

template <typename T>
inline uv2<T> operator-(const uv2<T> &lhs, const uv2<T> &rhs)
{
	uv2<T> out(lhs);
	out -= rhs;
	return out;
}

template<typename T>
uv2<T>& operator-=(uv2<T>& lhs, const uv2<T>& rhs)
{
	lhs.u -= rhs.u;
	lhs.v -= rhs.v;
	return lhs;
}

template <typename T>
inline uv2<T> operator-(const uv2<T> &vec)
{
	return uv2<T>(-vec.u, -vec.v);
}

}