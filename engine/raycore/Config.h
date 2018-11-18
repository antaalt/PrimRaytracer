#pragma once

// ------ INCLUDES -------

// Math
#include "geometry.h"
#include "Log.h"
using namespace geometry;

#include <cassert>
// ------- DEFINES -------
#define EPSILON				0.01f
#define DEFAULT_FOV			60.f
#define RAY_DEFAULT_DEPTH		10

// -------- DEBUG --------
#ifdef NDEBUG
#define ASSERT(condition, message)
#else
#define ASSERT(condition, message) assert((message, condition));
#endif

// ----- COORDINATES -----
// TODO move inline to .inl file (mat4.inl, vec3.inl, point3.inl OR geometry.inl)
namespace raycore {
	namespace settings {
		// Up on z axis
		const vec3 up = vec3(0.f, 0.f, 1.f);
		// Forward on y axis
		const vec3 forward = vec3(0.f, 1.f, 0.f);
		const vec3 right = vec3(1.f, 0.f, 0.f);
		const vec3 left = vec3(-1.f, 0.f, 0.f);
		// rotation is clockwise
	}
	struct Config {
		static int maxDepth;
		static float fov;
	};
}

