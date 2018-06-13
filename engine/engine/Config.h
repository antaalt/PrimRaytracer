#pragma once

// ------ INCLUDES -------

// Glew
#include <GL\glew.h>
// OpenGL
#include <GL\GL.h>
// GLM
#define GLM_FORCE_SWIZZLE
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\string_cast.hpp>

#include "Utils\Log.h"


#include <cassert>
// ------- DEFINES -------
#define OPENGL_MAJOR		3
#define OPENGL_MINOR		2
#define PROJECT_NAME		"projectEngine"
#define DEFAULT_WIDTH		800
#define DEFAULT_HEIGHT		600

#ifdef NDEBUG
#define ASSERT(condition, message)
#else
#define ASSERT(condition, message) assert((message, condition));
#endif