#pragma once

#include "color4.h"
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

template <typename T>
inline std::ostream& operator <<(std::ostream& os, const uv2<T>& vec)
{
	os << "uv2(" << vec.u << ", " << vec.v << ")";
	return os;
}
template <typename T>
inline std::ostream& operator <<(std::ostream& os, const vec3<T>& vec)
{
	os << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}
template <typename T>
inline std::ostream& operator <<(std::ostream& os, const vec4<T>& vec)
{
	os << "vec4(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
	return os;
}
template <typename T>
inline std::ostream& operator <<(std::ostream& os, const point3<T>& vec)
{
	os << "point3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}
template <typename T>
inline std::ostream& operator <<(std::ostream& os, const norm3<T>& vec)
{
	os << "norm3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}
template <typename T>
inline std::ostream& operator <<(std::ostream& os, const color4<T>& vec)
{
	os << "color4(" << vec.r << ", " << vec.g << ", " << vec.b << ", " << vec.a << ")";
	return os;
}
template <typename T>
inline std::ostream& operator <<(std::ostream& os, const quat<T>& vec)
{
	os << "quat(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
	return os;
}
template <typename T>
inline std::ostream& operator <<(std::ostream& os, const col3<T>& vec)
{
	os << "mat3::col(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
	return os;
}
template <typename T>
inline std::ostream& operator <<(std::ostream& os, const mat3<T>& vec)
{
	os << "mat3(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
	return os;
}
template <typename T>
inline std::ostream& operator <<(std::ostream& os, const col4<T>& vec)
{
	os << "col4(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ")";
	return os;
}
template <typename T>
inline std::ostream& operator <<(std::ostream& os, const mat4<T>& vec)
{
	os << "mat4(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ")";
	return os;
}

}
