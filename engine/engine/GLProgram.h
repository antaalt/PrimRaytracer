#pragma once

#include "Config.h"
#include "GLShader.h"

#include <map>

namespace Application {

	enum MatrixUniform {
		MATRIX_MODEL,
		MATRIX_VIEW,
		MATRIX_PROJECTION,
		NB_MATRIX_UNIFORMS
	};

	/*enum Uniform {
		TIME,
		NB_UNIFORMS
	};*/

#define NB_TOTAL_UNIFORM NB_UNIFORMS + NB_MATRIX_UNIFORMS

	const char *matrixUniformsName[NB_MATRIX_UNIFORMS] = {
		"matrix_model",
		"matrix_view",
		"matrix_projection"
	};

	const char *attributesName[World::NB_ATTRIBUTES] = {
		"in_position",
		"in_normal",
		/*"in_tangent",
		"in_bitangent",*/
		"in_color",
		"in_texcoord0",
		"in_texcoord1",
		"in_texcoord2",
		"in_texcoord3"
	};

	class GLProgram
	{
	public:
		GLProgram();
		~GLProgram();

		bool attachShader(GLShader::Type type, std::string path);

		bool linkProgram();

		void use();

		//bool updateFloatUniform(Uniform uniform, float value);
		bool hasMatrixUniform(MatrixUniform uniform);
		bool updateMatrixUniform(MatrixUniform uniform, const glm::mat4 &mat);
		/*bool updateVec4Uniform(Uniform uniform, const glm::vec4 &vec);
		bool updateVec3Uniform(Uniform uniform, const glm::vec3 &vec);*/
		
		static void doNotUse();
	private:
		GLShader *getShader(GLShader::Type type);
		GLShader *createShader(GLShader::Type type, std::string path);
	private:
		GLuint m_programID;
		GLShader *m_shaders[GLShader::NB_SHADERS];
		GLint m_matrixUniformsID[MatrixUniform::NB_MATRIX_UNIFORMS];

		bool m_valid;
		
	};

}