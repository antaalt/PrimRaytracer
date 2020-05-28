#pragma once
#include "scientific.h"

namespace geometry {

template <typename T>
struct norm3;
template <typename T>
struct vec3;
template <typename T>
struct col3;
template <typename T>
struct col4;

template <typename T>
struct point3 {
	union {
		T data[3];
		struct {
			T x, y, z;
		};
	};
	point3();
	explicit point3(T value);
	explicit point3(T x, T y, T z);
	explicit point3(const norm3<T> &normal);
	explicit point3(const col3<T> &col);
	explicit point3(const col4<T> &col);
	explicit point3(const vec3<T> &vec);

	T &operator[](size_t index);
	const T &operator[](size_t index) const;

	static T distance(const point3 &lhs, const point3 &rhs);
};


template <typename T>
bool operator==(const point3<T> &lhs, const point3<T> &rhs);
template <typename T>
bool operator!=(const point3<T> &lhs, const point3<T> &rhs);

template <typename T>
point3<T> operator*(const point3<T> &lhs, float rhs);
template <typename T>
point3<T> operator*(float lhs, const point3<T> &rhs);
template <typename T>
point3<T> &operator*=(point3<T> &lhs, float rhs);

template <typename T>
point3<T> operator/(const point3<T> &lhs, float rhs);
template <typename T>
point3<T> &operator/=(point3<T> &lhs, float rhs);

template <typename T>
point3<T> operator+(const point3<T> &lhs, const point3<T> &rhs);
template <typename T>
point3<T> &operator+=(point3<T> &lhs, const point3<T> &rhs);

template <typename T>
point3<T> operator-(const point3<T> &lhs, const point3<T> &rhs);
template <typename T>
point3<T> &operator-=(point3<T> &lhs, const point3<T> &rhs);

template <typename T>
point3<T> operator-(const point3<T> &vec);

}