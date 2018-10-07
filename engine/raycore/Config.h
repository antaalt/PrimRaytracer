#pragma once

// ------ INCLUDES -------

// Math
#include "Types.h"
#include "Utils\Log.h"


#include <cassert>
// ------- DEFINES -------
#define EPSILON				0.01f
#ifndef M_PI
#define M_PI				3.14159265358979323846
#endif
#define M_PIf				3.14159265358979323846f
#define DEFAULT_FOV			60.f

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

