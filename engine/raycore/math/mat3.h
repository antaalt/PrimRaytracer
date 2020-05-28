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
struct mat4;

template <typename T>
struct col3 {
	union {
		T data[3];
		struct {
			T x, y, z;
		};
	};
	col3();
	col3(T value);
	col3(T x, T y, T z);

	T &operator[](size_t index);
	const T &operator[](size_t index) const;
};

template <typename T>
struct mat3 {

	col3<T> cols[3];

	mat3();
	mat3(T value);
	mat3(const mat4<T>& mat);
	mat3(col3<T> x, col3<T> y, col3<T> z);

	col3<T> &operator[](size_t index);
	const col3<T> &operator[](size_t index) const;

	static mat3 identity();
};

template <typename T>
mat3<T> operator*(const mat3<T>& lhs, const mat3<T> &rhs);
template <typename T>
norm3<T> operator*(const mat3<T>& lhs, const norm3<T> &rhs);

}