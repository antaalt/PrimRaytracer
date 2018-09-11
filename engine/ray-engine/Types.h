#pragma once

#define USE_VECTOR
#define USE_MATRIX
#define USE_QUATERNION
#define USE_COLOR

#if defined(USE_VECTOR)
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
using ivec2 = Math::Vector2<int>;
using vec2 = Math::Vector2<float>;
using vec3 = Math::Vector3<float>;
using point3 = Math::Vector3<float>;
using norm3 = Math::Vector3<float>;
using uv2 = Math::Vector2<float>;
using vec4 = Math::Vector4<float>;
#if defined(USE_COLOR)
using color4 = Math::Vector4<float>;
#endif
#endif

#if defined(USE_MATRIX)
#include "Matrix4.h"
#include "Matrix3.h"
using mat4 = Math::Matrix4<float>;
using mat3 = Math::Matrix3<float>;
#endif

#if defined(USE_QUATERNION)
#include "Quaternion.h"
using quat = Math::Quaternion<float>;
#endif

namespace convert {
	template <typename T>
	Math::Vector3<T> toVec3(const Math::Vector4<T> &vec)
	{
		return Math::Vector3<T>(vec.x, vec.y, vec.z);
	}
	template <typename T>
	Math::Vector4<T> toVec4(const Math::Vector3<T> &vec, T val)
	{
		return Math::Vector4<T>(vec.x, vec.y, vec.z, val);
	}
#if defined(USE_QUATERNION) && defined(USE_MATRIX)
	template <typename T>
	Math::Matrix4<T> toMat4(const Math::Quaternion<T> &quat)
	{
		Math::Matrix4<T> m = Math::Matrix4<T>::identity();
		T sqw = quat.w*quat.w;
		T sqx = quat.x*quat.x;
		T sqy = quat.y*quat.y;
		T sqz = quat.z*quat.z;

		// invs (inverse square length) is only required if quaternion is not already normalised
		T invs = 1 / (sqx + sqy + sqz + sqw);
		m[1][1] = (sqx - sqy - sqz + sqw)*invs; // since sqw + sqx + sqy + sqz =1/invs*invs
		m[1][1] = (-sqx + sqy - sqz + sqw)*invs;
		m[2][2] = (-sqx - sqy + sqz + sqw)*invs;

		T tmp1 = quat.x * quat.y;
		T tmp2 = quat.z * quat.w;
		m[1][0] = T(2.0) * (tmp1 + tmp2)*invs;
		m[0][1] = T(2.0) * (tmp1 - tmp2)*invs;

		tmp1 = quat.x * quat.z;
		tmp2 = quat.y * quat.w;
		m[2][0] = T(2.0) * (tmp1 - tmp2)*invs;
		m[0][2] = T(2.0) * (tmp1 + tmp2)*invs;
		tmp1 = quat.y * quat.z;
		tmp2 = quat.x * quat.w;
		m[2][1] = T(2.0) * (tmp1 + tmp2)*invs;
		m[1][2] = T(2.0) * (tmp1 - tmp2)*invs;

		m[0][3] = T(0.0);
		m[1][3] = T(0.0);
		m[2][3] = T(0.0);
		m[3][0] = T(0.0);
		m[3][1] = T(0.0);
		m[3][2] = T(0.0);
		m[3][3] = T(1.0);
		return m;
	}

	template <typename T>
	Math::Matrix3<T> toMat3(const Math::Matrix4<T> &mat)
	{
		return Math::Matrix3<T>(
			toVec3(mat[0]),
			toVec3(mat[1]),
			toVec3(mat[2])
		);
	}
#endif
}