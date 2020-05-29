#pragma once
#include "scientific.h"
namespace geometry {

template <typename T>
struct point3;
template <typename T>
struct vec3;
template <typename T>
struct col3;
template <typename T>
struct col4;

template <typename T>
struct norm3 {
	union {
		T data[3];
		struct {
			T x, y, z;
		};
	};

	norm3();
	explicit norm3(T value);
	explicit norm3(T x, T y, T z);
	explicit norm3(const vec3<T> &vec);
	explicit norm3(const col3<T> &col);
	explicit norm3(const col4<T> &col);
	explicit norm3(const point3<T> &point);

	T &operator[](size_t index);
	const T &operator[](size_t index) const;

	T norm() const;

	static T dot(const norm3 &lhs, const norm3 &rhs);
	static norm3 normalize(const norm3 &vec);
	static norm3 faceForward(const norm3 &n, const vec3<T> &v);
};

template <typename T>
bool operator==(const norm3<T> &lhs, const norm3<T> &rhs);
template <typename T>
bool operator!=(const norm3<T> &lhs, const norm3<T> &rhs);

template <typename T>
norm3<T> operator*(const norm3<T> &lhs, float rhs);
template <typename T>
norm3<T> operator*(float lhs, const norm3<T> &rhs);
template <typename T>
norm3<T> &operator*=(norm3<T> &lhs, float rhs);

template <typename T>
norm3<T> operator/(const norm3<T> &lhs, float rhs);
template <typename T>
norm3<T> &operator/=(norm3<T> &lhs, float rhs);

template <typename T>
norm3<T> operator+(const norm3<T> &lhs, const norm3<T> &rhs);
template <typename T>
norm3<T> &operator+=(norm3<T> &lhs, const norm3<T> &rhs);

template <typename T>
norm3<T> operator-(const norm3<T> &lhs, const norm3<T> &rhs);
template <typename T>
norm3<T> &operator-=(norm3<T> &lhs, const norm3<T> &rhs);

template <typename T>
norm3<T> operator-(const norm3<T> &vec);


}