#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

namespace Application {

	using Vector3 = Math::Vector3<float>;
	using Point3 = Math::Vector3<float>;
	using Normal = Math::Vector3<float>;
	using Matrix4 = Math::Matrix4<float>;
	using Color32 = Math::Vector4<unsigned char>;
	using ColorHDR = Math::Vector4<float>;

	template <typename T>
	float dot(const Math::Vector3<T> &lhs, const Math::Vector3<T> &rhs);
	template <typename T>
	float cross(const Math::Vector3<T> &lhs, const Math::Vector3<T> &rhs);


	template <typename T>
	float dot(const Math::Vector3<T> & lhs, const Math::Vector3<T> & rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	template <typename T>
	float cross(const Math::Vector3<T> & lhs, const Math::Vector3<T> & rhs)
	{
		return 0.0f; // TODO cross product
	}

	template <typename T>
	Math::Vector3<T> normalize(const Math::Vector3<T> &vec)
	{
		return vec / vec.length();
	}

}