#pragma once

#include "scientific.h"

namespace geometry {

template <typename T>
struct vec2;

template <typename T>
struct uv2 {
	union {
		T data[2];
		struct {
			T u, v;
		};
	};
	uv2();
	explicit uv2(T value);
	explicit uv2(T u, T v);
	explicit uv2(const vec2<T> &vec);

	T &operator[](size_t index);
	const T &operator[](size_t index) const;
};

template <typename T>
bool operator==(const uv2<T> &lhs, const uv2<T> &rhs);
template <typename T>
bool operator!=(const uv2<T> &lhs, const uv2<T> &rhs);

template <typename T>
uv2<T> operator*(const uv2<T> &lhs, T rhs);
template <typename T>
uv2<T> operator*(T lhs, const uv2<T> &rhs);
template <typename T>
uv2<T> operator*=(uv2<T> &lhs, T rhs);

template <typename T>
uv2<T> operator/(const uv2<T> &lhs, T rhs);
template <typename T>
uv2<T> &operator/=(uv2<T> &lhs, T rhs);

template <typename T>
uv2<T> operator+(const uv2<T> &lhs, const uv2<T> &rhs);
template <typename T>
uv2<T> &operator+=(uv2<T> &lhs, const uv2<T> &rhs);

template <typename T>
uv2<T> operator-(const uv2<T> &lhs, const uv2<T> &rhs);
template <typename T>
uv2<T> &operator-=(uv2<T> &lhs, const uv2<T> &rhs);

template <typename T>
uv2<T> operator-(const uv2<T> &vec);

}