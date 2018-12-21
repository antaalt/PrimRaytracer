#pragma once
#include "color32.h"
#include "colorHDR.h"
#include "vec2.h"
#include "vec3.h"
#include "point3.h"
#include "norm3.h"
#include "uv2.h"
#include "mat3.h"
#include "mat4.h"
#include "quat.h"

#include <iostream>

namespace geometry {

	inline std::ostream& operator <<(std::ostream& os, const uv2& vec)
	{
		os << "uv2(" << vec.u << ", " << vec.v << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const vec3& vec)
	{
		os << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const point3& vec)
	{
		os << "point3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const norm3& vec)
	{
		os << "norm3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const colorHDR& vec)
	{
		os << "colorHDR(" << vec.r << ", " << vec.g << ", " << vec.b << ", " << vec.a << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const color32& vec)
	{
		os << "color32(" << vec.r << ", " << vec.g << ", " << vec.b << ", " << vec.a << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const quat& vec)
	{
		os << "quat(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const mat3::col& vec)
	{
		os << "mat3::col(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const mat3& vec)
	{
		os << "mat3(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const mat4::col& vec)
	{
		os << "mat4::col(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const mat4& vec)
	{
		os << "mat4(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ")";
		return os;
	}
}
