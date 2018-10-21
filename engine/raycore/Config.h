#pragma once

// ------ INCLUDES -------

// Math
#include "geometry.h"
#include "Utils\Log.h"
using namespace geometry;
using namespace math;

#include <cassert>
// ------- DEFINES -------
#define EPSILON				0.01f
#ifndef M_PI
#define M_PI				3.14159265358979323846
#endif
#define M_PIf				3.14159265358979323846f
#define DEFAULT_FOV			60.f
#define RAY_DEFAULT_DEPTH		10

#ifdef NDEBUG
#define ASSERT(condition, message)
#else
#define ASSERT(condition, message) assert((message, condition));
#endif

namespace raycore {
	struct Config {
		static int maxDepth;
		static float fov;
	};
}

