#include "quat.h"

namespace geometry {

template <typename T>
inline quat<T>::quat()
{
}

template <typename T>
inline quat<T>::quat(T x, T y, T z, T w) : 
	x(x), y(y), z(z), w(w)
{
}

template <typename T>
inline T & quat<T>::operator[](size_t index)
{
	return data[index];
}

template <typename T>
inline const T & quat<T>::operator[](size_t index) const
{
	return data[index];
}

template <typename T>
inline T quat<T>::norm() const
{
	return sqrt(x*x + y*y + z*z + w*w);
}

template <typename T>
inline quat<T> quat<T>::identity()
{
	return quat(0.f, 0.f, 0.f, 1.f);
}

template <typename T>
inline quat<T> quat<T>::conjuguate(const quat & quaternion)
{
	return quat(
		-quaternion.x, 
		-quaternion.y,
		-quaternion.z,
		quaternion.w
	);
}

template <typename T>
inline quat<T> quat<T>::normalize(const quat & quaternion)
{
	T n = quaternion.norm();
	return quat(
		quaternion.x / n,
		quaternion.y / n,
		quaternion.z / n,
		quaternion.w / n
	);
}

template <typename T>
inline quat<T> quat<T>::axis(const vec3<T> & axis, const radian<T> & angle)
{
	T sinHalfAngle = sin(angle / T(2));
	return quat(
		axis.x * sinHalfAngle,
		axis.y * sinHalfAngle,
		axis.z * sinHalfAngle,
		cos(angle / T(2))
	);
}

template <typename T>
inline quat<T> quat<T>::operator*(float scalar) const
{
	quat out(*this);
	out *= scalar;
	return out;
}

template <typename T>
inline quat<T> & quat<T>::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

template <typename T>
inline quat<T> quat<T>::operator*(const quat & rhs) const
{
	quat out(*this);
	out *= rhs;
	return out;
}

template <typename T>
inline quat<T> & quat<T>::operator*=(const quat & rhs)
{
	x =  x * rhs.w + y * rhs.z - z * rhs.y + w * rhs.x;
	y = -x * rhs.z + y * rhs.w + z * rhs.x + w * rhs.y;
	z =  x * rhs.y - y * rhs.x + z * rhs.w + w * rhs.z;
	w = -x * rhs.x - y * rhs.y - z * rhs.z + w * rhs.w;
	return *this;
}

template <typename T>
inline quat<T> quat<T>::operator+(const quat & rhs) const
{
	quat out(*this);
	out += rhs;
	return out;
}

template <typename T>
inline quat<T> & quat<T>::operator+=(const quat & rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

template <typename T>
inline quat<T> quat<T>::operator-(const quat & rhs) const
{
	quat out(*this);
	out -= rhs;
	return out;
}

template <typename T>
inline quat<T> & quat<T>::operator-=(const quat & rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}

template <typename T>
inline vec3<T> quat<T>::operator*(const vec3<T> & rhs)
{
	quat pcq = quat(rhs.x, rhs.y, rhs.z, 0.f) * quat(-x, -y, -z, w);
	quat q = *this * pcq;
	return vec3<T>(q.x, q.y, q.z);
}

}