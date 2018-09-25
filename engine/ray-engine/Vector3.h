#pragma once

#include <iostream>

namespace Math {

	template <typename T>
	struct Vector3 {
		union {
			T data[3];
			struct {
				T x, y, z;
			};
		};
		Vector3();
		Vector3(T x);
		Vector3(T x, T y, T z);
		template <typename U>
		Vector3(U x, U y, U z);
		template <typename U>
		Vector3(const Vector3<U> &vec);
		T &operator[](unsigned int index) { return data[index]; }
		const T &operator[](unsigned int index) const { return data[index]; }

		// comparison
		bool operator<(const Vector3<T> &v) const;
		bool operator==(const Vector3<T> &v) const;
		bool operator!=(const Vector3<T> &v) const;

		// arithmetic operator
		template <typename T>
		friend Vector3<T> operator*(const Vector3<T> &vec, float scalar);
		template <typename T>
		friend Vector3<T> operator*(float scalar, const Vector3<T> &vec);
		template <typename T>
		friend Vector3<T> operator/(const Vector3<T> &vec, float scalar);
		template <typename T>
		friend Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs);
		template <typename T>
		friend Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs);
		template <typename T>
		friend Vector3<T> operator-(const Vector3<T>& vec);

		// Functions
		float length() const;
			
		static T dot(const Vector3<T> &lhs, const Vector3<T> &rhs);
		static Vector3<T> cross(const Vector3<T> &lhs, const Vector3<T> &rhs);
		static Vector3<T> normalize(const Vector3<T> &vec);
		static float distance(const Vector3<T> &lhs, const Vector3<T> &rhs);

		template <typename T>
		friend std::ostream& operator <<(std::ostream& os, const Vector3<T>& vec);
	};

	template<typename T>
	inline Vector3<T>::Vector3()
	{
	}

	template<typename T>
	inline Vector3<T>::Vector3(T x) : x(x), y(x), z(x)
	{
	}

	template<typename T>
	inline Vector3<T>::Vector3(T x, T y, T z) : x(x), y(y), z(z)
	{
	}

	template<typename T>
	template<typename U>
	inline Vector3<T>::Vector3(U x, U y, U z) : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z))
	{
	}

	template<typename T>
	template <typename U>
	inline Vector3<T>::Vector3(const Vector3<U> &vec) : x(static_cast<T>(vec.x)), y(static_cast<T>(vec.y)), z(static_cast<T>(vec.z))
	{

	}

	template<typename T>
	inline bool Vector3<T>::operator<(const Vector3<T>& v) const
	{
		// TODO implement
		return false;
	}

	template<typename T>
	inline bool Vector3<T>::operator==(const Vector3<T>& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	template<typename T>
	inline bool Vector3<T>::operator!=(const Vector3<T>& v) const
	{
		return !(*this == v);
	}

	

	template<typename T>
	inline float Vector3<T>::length() const
	{
		return std::sqrtf(x*x + y*y + z*z);
	}

	template<typename T>
	inline T Vector3<T>::dot(const Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::cross(const Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		return Vector3<T>(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
			/*lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x*/
		);
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::normalize(const Vector3<T>& vec)
	{
		return vec / vec.length();
	}

	template<typename T>
	inline float Vector3<T>::distance(const Vector3<T> &lhs, const Vector3<T> &rhs)
	{
		return (rhs - lhs).length();
	}

	template<typename T>
	Vector3<T> operator*(const Vector3<T>& lhs, float scalar)
	{
		return Vector3<T>(lhs.x * scalar, lhs.y * scalar, lhs.z * scalar);
	}

	template<typename T>
	Vector3<T> operator*(float scalar, const Vector3<T>& vec)
	{
		return vec * scalar;
	}

	template<typename T>
	Vector3<T> operator/(const Vector3<T>& lhs, float scalar)
	{
		return Vector3<T>(lhs.x / scalar, lhs.y / scalar, lhs.z / scalar);
	}

	template<typename T>
	Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		return Vector3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	template<typename T>
	Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		return Vector3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	template<typename T>
	Vector3<T> operator-(const Vector3<T>& vec)
	{
		return Vector3<T>(-vec.x, -vec.y, -vec.z);
	}

	template<typename T>
	std::ostream & operator<<(std::ostream & os, const Vector3<T>& vec)
	{
		os << vec.x << ", " << vec.y << ", " << vec.z;
		return os;
	}

}
