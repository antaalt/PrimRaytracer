#pragma once

namespace geometry {

template <typename T> struct radian;
template <typename T> struct degree;

template <typename T>
struct degree {
	explicit degree();
	explicit degree(T value);
	explicit degree(const radian<T> &rad);
	template <typename U>
	explicit degree(const radian<U> &rad);

	const T &operator()() const;
	T &operator()();
private:
	T m_value;
};

template <typename T>
struct radian {
	explicit radian();
	explicit radian(T value);
	radian(const degree<T> &deg);
	template <typename U>
	radian(const degree<U> &deg);

	const T &operator()() const;
	T &operator()();
private:
	T m_value;
};


template <typename T>
radian<T> operator/(const radian<T> &rad, float value);
template <typename T>
radian<T> &operator/=(radian<T> &rad, float value);

// Values
template <typename T>
const radian<T> pi = T(3.14159265358979323846);

}