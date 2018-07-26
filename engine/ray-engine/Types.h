#pragma once

#define USE_VECTOR
#define USE_MATRIX
#define USE_QUATERNION
#define USE_COLOR

#if defined(USE_VECTOR)
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
using Vector3 = Math::Vector3<float>;
using Point3 = Math::Vector3<float>;
using Normal = Math::Vector3<float>;
using Texcoord = Math::Vector2<float>;
#if defined(USE_COLOR)
using Color32 = Math::Vector4<unsigned char>;
using ColorHDR = Math::Vector4<float>;
#endif
#endif

#if defined(USE_MATRIX)
#include "Matrix4.h"
using Matrix4 = Math::Matrix4<float>;
#endif

#if defined(USE_QUATERNION)
// TODO
#endif