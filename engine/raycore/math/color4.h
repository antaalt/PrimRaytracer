#pragma once

#include "scientific.h"

namespace geometry {

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

	T &operator[](size_t index);
	const T &operator[](size_t index) const;

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
color4<T> operator*(const color4<T> &lhs, T rhs);
template <typename T>
color4<T> operator*(T rhs, const color4<T> &lhs);
template <typename T>
color4<T> &operator*=(color4<T> &lhs, const color4<T> &rhs);
template <typename T>
color4<T> &operator*=(color4<T> &lhs, T rhs);

template <typename T>
color4<T> operator/(const color4<T> &lhs, T rhs);
template <typename T>
color4<T> &operator/=(color4<T> &lhs, T rhs);

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