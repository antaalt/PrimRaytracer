#include "Program.h"

#include "Scene\Mesh.h"

#include <vector>

namespace Application {
	namespace GL {
		Program::Program()
		{
			m_programID = glCreateProgram();
		}


		Program::~Program()
		{
			glDeleteProgram(m_programID);
			for (auto it = m_shaders.begin(); it != m_shaders.end(); it++)
				delete it->second;
		}

		bool Program::attachShader(GL::Shader::Type type, std::string path)
		{

			if (getShader(type) != nullptr)
			{
				Log::error("Shader already attached to program '" + GL::Shader::toString(type) + "'");
				return false;
			}
			GL::Shader *shader = createShader(type, path);
			if (!shader->isOk())
			{
				Log::error("Shader not loaded '" + path + "'");
				return false;
			}
			Log::info("Shader attached to program : " + GL::Shader::toString(type));
			const GLuint shaderID = shader->getID();
			if (!shaderID)
				return false;
			glAttachShader(m_programID, shaderID);
			return true;
		}

		bool Program::linkProgram()
		{
			for (int i = 0; i < Attributes::NB_ATTRIBUTES; i++)
				glBindAttribLocation(m_programID, i, attributesNames(static_cast<Attributes>(i)));

			// link program
			glLinkProgram(m_programID);
			GLint linked;
			glGetProgramiv(m_programID, GL_LINK_STATUS, &linked);
			if (linked == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> errorLog(maxLength);
				glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);
				std::string str(errorLog.begin(), errorLog.end());
				Log::error(str);
				// Exit with failure.
				glDeleteProgram(m_programID); // Don't leak the program.
				return false;
			}
			// Always detach shaders after a successful link.
			for (auto it = m_shaders.begin(); it != m_shaders.end(); it++)
			{
				GL::Shader *shader = it->second;
				glDetachShader(m_programID, shader->getID());
				// TODO need to delete it ? https://www.khronos.org/opengl/wiki/Shader_Compilation
				delete shader;
			}
			m_shaders.clear();

			// get uniforms locations
			use();
			for (int i = 0; i < NB_MATRIX_UNIFORMS; i++)
				m_matrixUniformsID[i] = glGetUniformLocation(m_programID, matrixUniformsNames(static_cast<MatrixUniform>(i)));
			doNotUse();

			return true;
		}
		void Program::use()
		{
			glUseProgram(m_programID);
		}
		bool Program::hasMatrixUniform(MatrixUniform uniform)
		{
			if (!isValid())
				return false;
			if (m_matrixUniformsID[uniform] == -1)
				return false;
			return true;
		}
		bool Program::updateMatrixUniform(MatrixUniform uniform, const glm::mat4 & mat)
		{
			if (!hasMatrixUniform(uniform))
				return false;
			glUniformMatrix4fv(m_matrixUniformsID[uniform], 1, GL_FALSE, &mat[0][0]);
			return true;
		}
		bool Program::isValid()
		{
			GLint isValid;
			glGetProgramiv(m_programID, GL_VALIDATE_STATUS, &isValid);
			return (isValid == GL_TRUE);
		}
		void Program::doNotUse()
		{
			glUseProgram(0);
		}
		GL::Shader * Program::getShader(GL::Shader::Type type)
		{
			auto it = m_shaders.find(type);
			if (it == m_shaders.end())
				return nullptr;
			return it->second;
		}
		GL::Shader * Program::createShader(GL::Shader::Type type, std::string path)
		{
			if (getShader(type) == nullptr)
				return m_shaders.insert(std::make_pair(type, new GL::Shader(type, path))).first->second;
			Log::error("Shader already created");
			return nullptr;
		}
		const char * matrixUniformsNames(MatrixUniform mat)
		{
			switch (mat)
			{
			case MatrixUniform::MATRIX_MODEL:
				return "matrix_model";
			case MatrixUniform::MATRIX_VIEW:
				return "matrix_view";
			case MatrixUniform::MATRIX_PROJECTION:
				return "matrix_projection";
			case MatrixUniform::MATRIX_MVP:
				return "matrix_mvp";
			default:
				return "";
			}
		}
}
}