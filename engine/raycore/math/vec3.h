#pragma once
#include "scientific.h"

namespace geometry {

template <typename T>
struct norm3;
template <typename T>
struct point3;
template <typename T>
struct col3;
template <typename T>
struct col4;
template <typename T>
struct vec2;
template <typename T>
struct vec4;


template <typename T>
struct vec3 {
	union {
		T data[3];
		struct {
			T x, y, z;
		};
	};
	vec3();
	explicit vec3(T value);
	explicit vec3(T x, T y, T z);
	explicit vec3(const norm3<T> &normal);
	explicit vec3(const point3<T> &point);
	explicit vec3(const col3<T> &col);
	explicit vec3(const col4<T> &col);
	explicit vec3(const vec2<T> &vec, T z);
	explicit vec3(const vec4<T> &vec);

	T &operator[](size_t index);
	const T &operator[](size_t index) const;

	T norm() const;

	static vec3 normalize(const vec3 &vec);
	static T dot(const vec3 & lhs, const vec3 & rhs);
	static vec3 cross(const vec3 & lhs, const vec3 & rhs);
};

template <typename T>
bool operator==(const vec3<T> & lhs, const vec3<T> & rhs);
template <typename T>
bool operator!=(const vec3<T> &lhs, const vec3<T> &rhs);

template <typename T>
vec3<T> operator*(const vec3<T> &lhs, T rhs);
template <typename T>
vec3<T> operator*(T lhs, const vec3<T> &rhs);
template <typename T>
vec3<T> &operator*=(vec3<T> &lhs, T rhs);

template <typename T>
vec3<T> operator/(const vec3<T> &lhs, T rhs);
template <typename T>
vec3<T> &operator/=(vec3<T> &lhs, T rhs);

template <typename T>
vec3<T> operator+(const vec3<T> &lhs, const vec3<T> &rhs);
template <typename T>
vec3<T> &operator+=(vec3<T> &lhs, const vec3<T> &rhs);

template <typename T>
vec3<T> operator-(const vec3<T> &lhs, const vec3<T> &rhs);
template <typename T>
vec3<T> &operator-=(vec3<T> &lhs, const vec3<T> &rhs);

template <typename T>
vec3<T> operator-(const vec3<T> &vec);

}