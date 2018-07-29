#pragma once
#include "Vector4.h"


namespace Math {

	template <typename T>
	struct Quaternion : Vector4<T> {
		Quaternion();
		Quaternion(T x, T y, T z, T w);

		static Quaternion<T> identity();
	};
	template<typename T>
	inline Quaternion<T>::Quaternion()
	{
	}
	template<typename T>
	inline Quaternion<T>::Quaternion(T x, T y, T z, T w) : Vector4<T>(x, y, z, w)
	{
	}
	template<typename T>
	inline Quaternion<T> Quaternion<T>::identity()
	{
		return Quaternion<T>(0.f, 0.f, 0.f, 1.f);
	}
}