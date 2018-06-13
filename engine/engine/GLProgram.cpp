#include "GLProgram.h"

#include "Scene\Mesh.h"

#include <vector>

namespace Application {
	GLProgram::GLProgram() : m_valid(false)
	{
		m_programID = glCreateProgram();
		for (int i = 0; i < GLShader::NB_SHADERS; i++)
			m_shaders[i] = nullptr;
	}


	GLProgram::~GLProgram()
	{
		glDeleteProgram(m_programID);
	}

	bool GLProgram::attachShader(GLShader::Type type, std::string path)
	{
		
		if (getShader(type) != nullptr)
		{
			Log::error("Shader already attached to program '" + GLShader::toString(type) + "'");
			return false;
		}
		GLShader *shader = createShader(type, path);
		if (!shader->isOk())
		{
			Log::error("Shader not loaded '" + path + "'");
			return false;
		}
		Log::info("Shader attached to program : " + GLShader::toString(type));
		const GLuint shaderID = shader->getID();
		if (!shaderID)
			return false;
		glAttachShader(m_programID, shaderID);
		return true;
	}

	bool GLProgram::linkProgram()
	{
		for (int i = 0; i < World::Attributes::NB_ATTRIBUTES; i++)
			glBindAttribLocation(m_programID, i, attributesName[i]);

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
		for (int iShader = 0; iShader < GLShader::NB_SHADERS; iShader++)
		{
			if (m_shaders[iShader] == nullptr)
				continue;
			glDetachShader(m_programID, m_shaders[iShader]->getID());
			// TODO need to delete it ? https://www.khronos.org/opengl/wiki/Shader_Compilation
			/*delete m_shaders[iShader];
			m_shaders[iShader] = nullptr;*/
		}

		// get uniforms locations
		use();
		for (int i = 0; i < NB_MATRIX_UNIFORMS; i++)
			m_matrixUniformsID[i] = glGetUniformLocation(m_programID, matrixUniformsName[i]);
		doNotUse();

		m_valid = true;

		return true;
	}
	void GLProgram::use()
	{
		glUseProgram(m_programID);
	}
	bool GLProgram::hasMatrixUniform(MatrixUniform uniform)
	{
		if (!m_valid)
			return false;
		if (m_matrixUniformsID[uniform] == -1);
		return false;
	}
	bool GLProgram::updateMatrixUniform(MatrixUniform uniform, const glm::mat4 & mat)
	{
		if (!hasMatrixUniform(uniform))
			return false;
		glUniformMatrix4fv(uniform, 1, GL_FALSE, &mat[0][0]);
		return true;
	}
	void GLProgram::doNotUse()
	{
		glUseProgram(0);
	}
	GLShader * GLProgram::getShader(GLShader::Type type)
	{
		const int index = GLShader::value(type);
		return m_shaders[index];
	}
	GLShader * GLProgram::createShader(GLShader::Type type, std::string path)
	{
		const int index = GLShader::value(type);
		m_shaders[index] = new GLShader(type, path);
		return m_shaders[index];
	}
}
