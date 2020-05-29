#pragma once

#include "angle.h"

namespace geometry {

template <typename T>
inline degree<T>::degree() :
	m_value(T(0))
{
}

template <typename T>
inline degree<T>::degree(T value) :
	m_value(value)
{
}

template <typename T>
inline degree<T>::degree(const radian<T> &rad) :
	m_value(rad() / pi<T>() * T(180))
{
}

template <typename T>
template <typename U>
inline degree<T>::degree(const radian<U> &rad) :
	degree(radian<T>(static_cast<T>(rad.m_value)))
{
}

template <typename T>
const T &degree<T>::operator()() const
{
	return m_value;
}

template <typename T>
T &degree<T>::operator()()
{
	return m_value;
}

template <typename T>
inline radian<T>::radian() :
	m_value(T(0))
{
}
template <typename T>
inline radian<T>::radian(T value) :
	m_value(value)
{
}

template <typename T>
inline radian<T>::radian(const degree<T> &deg) :
	m_value(deg() / T(180) * pi<T>())
{
}

template <typename T>
template <typename U>
inline radian<T>::radian(const degree<U> &deg) :
	radian(degree<T>(static_cast<T>(deg.m_value)))
{
}

template <typename T>
const T &radian<T>::operator()() const
{
	return m_value;
}

template <typename T>
T &radian<T>::operator()()
{
	return m_value;
}

template<typename T>
T operator/(const radian<T>& lhs, const radian<T>& rhs)
{
	return lhs() / rhs();
}

template <typename T>
radian<T> operator/(const radian<T> &rad, float value)
{
	radian<T> out(rad);
	out /= value;
	return out;
}

template <typename T>
radian<T> &operator/=(radian<T> &rad, float value)
{
	rad() /= value;
	return rad;
}

template <typename T>
radian<T> operator*(const radian<T> &rad, float value)
{
	radian<T> out(rad);
	out *= value;
	return out;
}

template <typename T>
radian<T> operator*(float value, const radian<T> &rad)
{
	radian<T> out(rad);
	out *= value;
	return out;
}

template <typename T>
radian<T> &operator*=(radian<T> &rad, float value)
{
	rad() *= value;
	return rad;
}

}