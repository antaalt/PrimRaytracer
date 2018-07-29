#pragma once

#include "Config.h"


/*#if OPENGL_MAJOR < 4// requires GL 4.0 or ARB_tessellation_shader
	#define NB_SHADERS 3
#else
	#if OPENGL_MINOR < 3 // require GL 4.3 or ARB_compute_shader
		#define NB_SHADERS 5
	#else
		#define NB_SHADERS 6
	#endif
#endif*/
#define NB_SHADERS 6

namespace app {

	namespace GL {
		class Shader
		{
		public:
			enum Type {
				VERTEX_SHADER = GL_VERTEX_SHADER,
				TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
				TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
				FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
				GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
				COMPUTE_SHADER = GL_COMPUTE_SHADER
			};
			static std::string toString(Type type);

		public:
			Shader(Type type, std::string path);
			~Shader();

			GLuint getID() const;

			bool isOk() const;

		private:
			bool compileShaderFromContent(std::string content);
			std::string loadShaderFromFile(std::string path);

		private:
			Type m_shaderType;
			GLuint m_shaderID;
		};
	}
}