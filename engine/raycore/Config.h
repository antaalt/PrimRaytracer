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

#ifdef NDEBUG
#define ASSERT(condition, message)
#else
#define ASSERT(condition, message) assert((message, condition));
#endif