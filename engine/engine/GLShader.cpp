#include "GLShader.h"

#include <fstream>
#include <vector>

namespace Application {
	std::string GLShader::toString(Type type)
	{
		switch (type)
		{
		case Type::VERTEX_SHADER:
			return "VERTEX_SHADER";
		case Type::TESS_CONTROL_SHADER:
			return "TESS_CONTROL_SHADER";
		case Type::TESS_EVALUATION_SHADER:
			return "TESS_EVALUATION_SHADER";
		case Type::FRAGMENT_SHADER:
			return "FRAGMENT_SHADER";
		case Type::GEOMETRY_SHADER:
			return "GEOMETRY_SHADER";
		case Type::COMPUTE_SHADER:
			return "COMPUTE_SHADER";
		}
	}

	int GLShader::value(Type type)
	{
		switch (type)
		{
		case GLShader::VERTEX_SHADER:
			return 0;
		case GLShader::TESS_CONTROL_SHADER:
			return 1;
		case GLShader::TESS_EVALUATION_SHADER:
			return 2;
		case GLShader::FRAGMENT_SHADER:
			return 3;
		case GLShader::GEOMETRY_SHADER:
			return 4;
		case GLShader::COMPUTE_SHADER:
			return 5;
		default:
			return 0;
		}
	}

	GLShader::GLShader(Type type, std::string path) : m_shaderType(type)
	{
		std::string content = loadGLShaderFromFile(path);
		compileGLShaderFromContent(content);
	}

	GLShader::~GLShader()
	{

		glDeleteShader(m_shaderID);
	}

	std::string GLShader::loadGLShaderFromFile(std::string path)
	{
		std::ifstream ifs(path);
		if (!ifs) {
			Log::error("File " + path + " could not be open");
			return "";
		}
		std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

		return content;
	}

	GLuint GLShader::getID() const
	{
		return m_shaderID;
	}

	bool GLShader::isOk() const
	{
		return (glIsShader(m_shaderID) == GL_TRUE);
	}

	bool GLShader::compileGLShaderFromContent(std::string content)
	{
		m_shaderID = glCreateShader(m_shaderType);
		const char *shaderStr = content.c_str();
		glShaderSource(m_shaderID, 1, &shaderStr, NULL);
		glCompileShader(m_shaderID);
		GLint isCompiled;
		glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(m_shaderID, maxLength, &maxLength, &errorLog[0]);
			std::string str(errorLog.begin(), errorLog.end());
			Log::error(str);
			// Exit with failure.
			glDeleteShader(m_shaderID); // Don't leak the shader.
			return false;
		}
		return true;
	}

}