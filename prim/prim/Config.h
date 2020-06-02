#pragma once

// ------ INCLUDES -------

// Math
#include "math/geometry.h"
#include "Log.h"
using namespace geometry;
namespace geo = geometry;

#include <cassert>

// -------- DEBUG --------
#ifdef NDEBUG
#define ASSERT(condition, message)
#else
#define ASSERT(condition, message) assert((message, condition));
#endif


