#pragma once

#include "gpu.h"
#include "scientific.h"
#include "sampling.h"
#include "angle.h"
#include "color4.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "point3.h"
#include "norm3.h"
#include "uv2.h"
#include "mat3.h"
#include "mat4.h"
#include "quat.h"
#include "print.h"

namespace geometry {

using quatf = quat<float>;
using quatd = quat<double>;

using uv2f = uv2<float>;
using uv2d = uv2<double>;

using point3f = point3<float>;
using point3d = point3<double>;
using norm3f = norm3<float>;
using norm3d = norm3<double>;

using vec2f = vec2<float>;
using vec2d = vec2<double>;
using vec3f = vec3<float>;
using vec3d = vec3<double>;
using vec4f = vec4<float>;
using vec4d = vec4<double>;

using color4f = color4<float>;
using color32 = color4<uint8_t>;

using col3f = col3<float>;
using col3d = col3<double>;
using mat3f = mat3<float>;
using mat3d = mat3<double>;

using col4f = col4<float>;
using col4d = col4<double>;
using mat4f = mat4<float>;
using mat4d = mat4<double>;

using degreef = degree<float>;
using radianf = radian<float>;

}

#include "gpu.inl"
#include "scientific.inl"
#include "sampling.inl"
#include "angle.inl"
#include "color4.inl"
#include "vec2.inl"
#include "vec3.inl"
#include "vec4.inl"
#include "point3.inl"
#include "norm3.inl"
#include "uv2.inl"
#include "mat3.inl"
#include "mat4.inl"
#include "quat.inl"