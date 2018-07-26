#pragma once

#include <iostream>

namespace Math {

	template <typename T>
	struct Vector2 {
		union {
			T data[2];
			struct {
				T x, y;
			};
		};
		Vector2();
		Vector2(T x);
		Vector2(T x, T y);
		T &operator[](unsigned int index) { return data[index]; }
		const T &operator[](unsigned int index) const { return data[index]; }

		// comparison
		bool operator<(const Vector2<T> &v) const;
		bool operator==(const Vector2<T> &v) const;
		bool operator!=(const Vector2<T> &v) const;

		// arithmetic operator
		template <typename T>
		friend Vector2<T> operator*(const Vector2<T> &vec, float scalar);
		template <typename T>
		friend Vector2<T> operator*(float scalar, const Vector2<T> &vec);
		template <typename T>
		friend Vector2<T> operator/(const Vector2<T> &vec, float scalar);
		template <typename T>
		friend Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs);
		template <typename T>
		friend Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs);

		// Functions
		float length() const;

		static T dot(const Vector2<T> &lhs, const Vector2<T> &rhs);
		static Vector2<T> normalize(const Vector2<T> &vec);

		template <typename T>
		friend std::ostream& operator <<(std::ostream& os, const Vector2<T>& vec);
	};

	template<typename T>
	inline Vector2<T>::Vector2()
	{
	}

	template<typename T>
	inline Vector2<T>::Vector2(T x) : x(x), y(x)
	{
	}

	template<typename T>
	inline Vector2<T>::Vector2(T x, T y) : x(x), y(y)
	{
	}

	template<typename T>
	inline bool Vector2<T>::operator<(const Vector2<T>& v) const
	{
		// TODO implement
		return false;
	}

	template<typename T>
	inline bool Vector2<T>::operator==(const Vector2<T>& v) const
	{
		return x == v.x && y == v.y;
	}

	template<typename T>
	inline bool Vector2<T>::operator!=(const Vector2<T>& v) const
	{
		return !(*this == v);
	}

	template<typename T>
	inline float Vector2<T>::length() const
	{
		return std::sqrtf(x * x + y * y);
	}

	template<typename T>
	inline T Vector2<T>::dot(const Vector2<T>& lhs, const Vector2<T>& rhs)
	{
		return x * x + y * y;
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::normalize(const Vector2<T>& vec)
	{
		return vec / vec.length();
	}

	template<typename T>
	Vector2<T> operator*(const Vector2<T>& lhs, float scalar)
	{
		return Vector2<T>(lhs.x * scalar, lhs.y * scalar);
	}

	template<typename T>
	Vector2<T> operator*(float scalar, const Vector2<T>& vec)
	{
		return vec * scalar;
	}

	template<typename T>
	Vector2<T> operator/(const Vector2<T>& lhs, float scalar)
	{
		return Vector2<T>(lhs.x / scalar, lhs.y / scalar);
	}

	template<typename T>
	Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs)
	{
		return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	template<typename T>
	Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs)
	{
		return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	template<typename T>
	std::ostream & operator<<(std::ostream & os, const Vector2<T>& vec)
	{
		os << vec.x << ", " << vec.y;
		return os;
	}

}