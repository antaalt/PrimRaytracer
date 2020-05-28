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

}