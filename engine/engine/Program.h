#pragma once

#include "Config.h"
#include "Enum.h"
#include "Shader.h"

#include <map>

namespace app {
	namespace GL {

		enum MatrixUniform {
			MATRIX_MODEL,
			MATRIX_VIEW,
			MATRIX_PROJECTION,
			MATRIX_MVP,
			NB_MATRIX_UNIFORMS
		};

		enum Uniform {
			TIME_UNIFORM
		};

		enum VectorUniform {
			LIGHT_UNIFORM,
		};

		const char* matrixUniformsNames(MatrixUniform mat);

		class Program
		{
		public:
			Program();
			~Program();

			bool attachShader(GL::Shader::Type type, std::string path);

			bool linkProgram();

			void use();

			bool hasMatrixUniform(MatrixUniform uniform);
			bool updateMatrixUniform(MatrixUniform uniform, const glm::mat4 &mat);

			bool isValid();

			static void doNotUse();
		private:
			GL::Shader *getShader(GL::Shader::Type type);
			GL::Shader *createShader(GL::Shader::Type type, std::string path);
		private:
			GLuint m_programID;
			std::map<GL::Shader::Type, GL::Shader*> m_shaders;
			GLint m_matrixUniformsID[MatrixUniform::NB_MATRIX_UNIFORMS];
		};
	}
}