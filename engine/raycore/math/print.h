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
		std::cout << "uv2(" << vec.u << ", " << vec.v << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const vec3& vec)
	{
		std::cout << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const point3& vec)
	{
		std::cout << "point3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const norm3& vec)
	{
		std::cout << "norm3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const colorHDR& vec)
	{
		std::cout << "colorHDR(" << vec.r << ", " << vec.g << ", " << vec.b << ", " << vec.a << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const color32& vec)
	{
		std::cout << "color32(" << vec.r << ", " << vec.g << ", " << vec.b << ", " << vec.a << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const quat& vec)
	{
		std::cout << "quat(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const mat3::col& vec)
	{
		std::cout << "mat3::col(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const mat3& vec)
	{
		std::cout << "mat3(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const mat4::col& vec)
	{
		std::cout << "mat4::col(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ")";
		return os;
	}
	inline std::ostream& operator <<(std::ostream& os, const mat4& vec)
	{
		std::cout << "mat4(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ")";
		return os;
	}
}