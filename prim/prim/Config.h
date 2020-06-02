#pragma once

// ------ INCLUDES -------

// Math
#include "math/geometry.h"
#include "Log.h"
using namespace geometry;
namespace geo = geometry;

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
// TODO move inline to .inl file (mat4.inl, geometry::vec3f.inl, point3.inl OR geometry.inl)
namespace prim {
	namespace settings {
		// Up on z axis
		const geometry::vec3f up = geometry::vec3f(0.f, 0.f, 1.f);
		// Forward on y axis
		const geometry::vec3f forward = geometry::vec3f(0.f, 1.f, 0.f);
		const geometry::vec3f right = geometry::vec3f(1.f, 0.f, 0.f);
		const geometry::vec3f left = geometry::vec3f(-1.f, 0.f, 0.f);
		// rotation is clockwise
	}
	struct Config {
		static int maxDepth;
		static float fov;
	};
}

