#pragma once
#include "scientific.h"

namespace geometry {

template <typename T>
struct norm3;
template <typename T>
struct point3;
template <typename T>
struct vec3;
template <typename T>
struct quat;

template <typename T>
struct col4 {
	union {
		T data[4];
		struct {
			T x, y, z, w;
		};
	};
	col4();
	col4(T value);
	col4(T x, T y, T z, T w);
	col4(norm3<T> vec, T w);
	col4(vec3<T> vec, T w);
	col4(vec4<T> vec);
	col4(point3<T> vec, T w);

	T &operator[](size_t index);
	const T &operator[](size_t index) const;
};

template <typename T>
struct mat4 {

	col4<T> cols[4];

	mat4();
	mat4(T value);
	mat4(col4<T> x, col4<T> y, col4<T> z, col4<T> w);
	mat4(const quat<T> &quat);

	col4<T> &operator[](size_t index);
	const col4<T> &operator[](size_t index) const;

	static mat4 identity();
	static mat4 translate(const vec3<T> &translation);
	static mat4 rotate(const vec3<T> &axis, radian<T> angle);
	static mat4 scale(const vec3<T> &scale);
	static mat4 TRS(const vec3<T> & t, const quat<T> & r, const vec3<T> & s);
	static mat4 inverse(const mat4 &mat);
	static mat4 perspective(const radian<T> &fov, float ratio, float nearZ, float farZ);
	static mat4 lookAt(const point3<T> & eye, const point3<T> & target, const norm3<T> &up = norm3<T>(0, 1, 0));
	float det() const;
};

template <typename T>
mat4<T> operator*(const mat4<T>& lhs, const mat4<T> &rhs);
template <typename T>
mat4<T> &operator*=(mat4<T>& lhs, const mat4<T> &rhs);
template <typename T>
point3<T> operator*(const mat4<T>& lhs, const point3<T> &rhs);
template <typename T>
vec3<T> operator*(const mat4<T>& lhs, const vec3<T> &rhs);
template <typename T>
vec4<T> operator*(const mat4<T>& lhs, const vec4<T> &rhs);

}