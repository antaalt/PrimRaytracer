#pragma once

// ------ INCLUDES -------

// Glew
#include <GL\glew.h>
// OpenGL
#include <GL\GL.h>
// Math
//#include "Types.h"
#include "Utils\Log.h"


#include <cassert>
// ------- DEFINES -------
#define PROJECT_NAME		"RayEngine"
#define OPENGL_MAJOR		3
#define OPENGL_MINOR		2
#define DEFAULT_WIDTH		800
#define DEFAULT_HEIGHT		600


#ifdef NDEBUG
#define ASSERT(condition, message)
#else
#define ASSERT(condition, message) assert((message, condition));
#endif