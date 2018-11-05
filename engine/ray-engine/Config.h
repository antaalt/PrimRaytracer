#pragma once

// ------ INCLUDES -------

// Glew
#include <GL\glew.h>
// OpenGL
#include <GL\GL.h>

//#include "Utils\Log.h"
#include "../raycore/Config.h"

#include <cassert>
// ------- DEFINES -------
#define PROJECT_NAME		"RayEngine"
#define DEFAULT_WIDTH		800
#define DEFAULT_HEIGHT		600
#define GLOBAL_MAX_DEPTH	15


#ifdef NDEBUG
#define ASSERT(condition, message)
#else
#define ASSERT(condition, message) assert((message, condition));
#endif