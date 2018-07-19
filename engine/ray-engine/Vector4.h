#pragma once

#include <iostream>

namespace Application {


	namespace Math {

		template <typename T>
		struct Vector4 {
			union {
				T data[4];
				struct {
					T x, y, z, w;
				};
			};
			Vector4();
			Vector4(T x);
			Vector4(T x, T y, T z, T w);
			T &operator[](unsigned int index) { return data[index]; }
			const T &operator[](unsigned int index) const { return data[index]; }

			// comparison
			bool operator<(const Vector4<T> &v) const;
			bool operator==(const Vector4<T> &v) const;
			bool operator!=(const Vector4<T> &v) const;

			// arithmetic operator
			template <typename T>
			friend Vector4<T> operator*(const Vector4<T> &vec, float scalar);
			template <typename T>
			friend Vector4<T> operator*(float scalar, const Vector4<T> &vec);
			template <typename T>
			friend Vector4<T> operator/(const Vector4<T> &vec, float scalar);
			template <typename T>
			friend Vector4<T> operator+(const Vector4<T>& lhs, const Vector4<T>& rhs);
			template <typename T>
			friend Vector4<T> operator-(const Vector4<T>& lhs, const Vector4<T>& rhs);

			// Functions
			float length() const;

			template <typename T>
			friend std::ostream& operator <<(std::ostream& os, const Vector4<T>& vec);
		};

		template<typename T>
		inline Vector4<T>::Vector4()
		{
		}

		template<typename T>
		inline Vector4<T>::Vector4(T x) : x(x), y(x), z(x), w(x)
		{
		}

		template<typename T>
		inline Vector4<T>::Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
		{
		}

		template<typename T>
		inline bool Vector4<T>::operator<(const Vector4<T>& v) const
		{
			// TODO implement
			return false;
		}

		template<typename T>
		inline bool Vector4<T>::operator==(const Vector4<T>& v) const
		{
			return x == v.x && y == v.y && z == v.z && w == v.w;
		}

		template<typename T>
		inline bool Vector4<T>::operator!=(const Vector4<T>& v) const
		{
			return !(*this == v);
		}

		template<typename T>
		inline float Vector4<T>::length() const
		{
			return std::sqrtf(x * x + y * y + z * z + w * w);
		}

		template<typename T>
		Vector4<T> operator*(const Vector4<T>& lhs, float scalar)
		{
			return Vector4<T>(lhs.x * scalar, lhs.y * scalar, lhs.z * scalar, lhs.w * scalar);
		}

		template<typename T>
		Vector4<T> operator*(float scalar, const Vector4<T>& vec)
		{
			return vec * scalar;
		}

		template<typename T>
		Vector4<T> operator/(const Vector4<T>& lhs, float scalar)
		{
			return Vector4<T>(lhs.x / scalar, lhs.y / scalar, lhs.z / scalar, lhs.w / scalar);
		}

		template<typename T>
		Vector4<T> operator+(const Vector4<T>& lhs, const Vector4<T>& rhs)
		{
			return Vector4<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
		}

		template<typename T>
		Vector4<T> operator-(const Vector4<T>& lhs, const Vector4<T>& rhs)
		{
			return Vector4<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
		}

		template<typename T>
		std::ostream & operator<<(std::ostream & os, const Vector4<T>& vec)
		{
			os << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
			return os;
		}

	}
}

#pragma once
