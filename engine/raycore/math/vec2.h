#pragma once
#include "scientific.h"

namespace geometry {

template <typename T>
struct vec3;
template <typename T>
struct vec4;
template <typename T>
struct uv2;

template <typename T>
struct vec2 {
	union {
		T data[2];
		struct {
			T x, y;
		};
	};
	vec2();
	explicit vec2(T value);
	explicit vec2(T x, T y);
	explicit vec2(const uv2<T> &uv);
	explicit vec2(const vec3<T> &vec);
	explicit vec2(const vec4<T> &vec);

	T &operator[](size_t index);
	const T &operator[](size_t index) const;

	T norm() const;

	static T dot(const vec2 &lhs, const vec2 &rhs);
};

template <typename T>
bool operator==(const vec2<T> & lhs, const vec2<T> & rhs);
template <typename T>
bool operator!=(const vec2<T> &lhs, const vec2<T> &rhs);

template <typename T>
vec2<T> operator*(const vec2<T> &lhs, T rhs);
template <typename T>
vec2<T> operator*(T lhs, const vec2<T> &rhs);
template <typename T>
vec2<T> &operator*=(vec2<T> &lhs, T rhs);

template <typename T>
vec2<T> operator/(const vec2<T> &lhs, T rhs);
template <typename T>
vec2<T> &operator/=(vec2<T> &lhs, T rhs);

template <typename T>
vec2<T> operator+(const vec2<T> &lhs, const vec2<T> &rhs);
template <typename T>
vec2<T> &operator+=(vec2<T> &lhs, const vec2<T> &rhs);

template <typename T>
vec2<T> operator-(const vec2<T> &lhs, const vec2<T> &rhs);
template <typename T>
vec2<T> &operator-=(vec2<T> &lhs, const vec2<T> &rhs);

template <typename T>
vec2<T> operator-(const vec2<T> &vec);

}