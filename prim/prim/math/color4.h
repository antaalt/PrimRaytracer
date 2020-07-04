#pragma once

#include "scientific.h"

namespace geometry {

template <typename T>
struct vec3;
template <typename T>
struct vec4;

template <typename T>
struct color4 {
	union {
		T data[4];
		struct {
			T r, g, b, a;
		};
	};
	color4();
	explicit color4(T value);
	template <typename U>
	explicit color4(const color4<U> &value);
	explicit color4(T r, T g, T b, T a);
	explicit color4(const vec3<T> &vec, T a);
	explicit color4(const vec4<T> &vec);

	T &operator[](size_t index);
	const T &operator[](size_t index) const;

	T luminance() const;

	static color4<T> srgb2linear(const color4<T> &color);
	static color4<T> linear2srgb(const color4<T> &color);
};

template <typename T>
bool operator==(const color4<T> &lhs, const color4<T> &rhs);
template <typename T>
bool operator!=(const color4<T> &lhs, const color4<T> &rhs);

template <typename T>
color4<T> operator*(const color4<T> &lhs, const color4<T> &rhs);
template <typename T>
color4<T> operator*(const color4<T> &lhs, real_t rhs);
template <typename T>
color4<T> operator*(T rhs, const color4<T> &lhs);
template <typename T>
color4<T> &operator*=(color4<T> &lhs, const color4<T> &rhs);
template <typename T>
color4<T> &operator*=(color4<T> &lhs, real_t rhs);

template <typename T>
color4<T> operator/(const color4<T> &lhs, real_t rhs);
template <typename T>
color4<T> &operator/=(color4<T> &lhs, real_t rhs);

template <typename T>
color4<T> operator+(const color4<T> &lhs, const color4<T> &rhs);
template <typename T>
color4<T> &operator+=(color4<T> &lhs, const color4<T> &rhs);

template <typename T>
color4<T> operator-(const color4<T> &lhs, const color4<T> &rhs);
template <typename T>
color4<T> &operator-=(color4<T> &lhs, const color4<T> &rhs);

template <typename T>
color4<T> operator-(const color4<T> &vec);

}