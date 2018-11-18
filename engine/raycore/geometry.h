#pragma once

#include "math/color32.h"
#include "math/colorHDR.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/point3.h"
#include "math/norm3.h"
#include "math/uv2.h"
#include "math/mat3.h"
#include "math/mat4.h"
#include "math/quat.h"
#include "math/print.h"

namespace geometry {
	inline math::real_t dot(const vec3 & lhs, const norm3 & rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
	inline math::real_t dot(const point3 &lhs, const norm3 &rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
	inline vec3 operator-(const vec3 &lhs, const norm3 &rhs) { return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
	inline norm3 operator-(const norm3 &lhs, const vec3 &rhs) { return norm3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
	inline vec3 operator+(const point3 &lhs, const vec3 &rhs) { return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
}
#include "math/color32.inl"
#include "math/colorHDR.inl"
#include "math/vec2.inl"
#include "math/vec3.inl"
#include "math/point3.inl"
#include "math/norm3.inl"
#include "math/uv2.inl"
#include "math/mat3.inl"
#include "math/mat4.inl"
#include "math/quat.inl"