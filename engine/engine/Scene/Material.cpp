#include "Material.h"

namespace World {

	Material::Material()
	{
	}


	Material::~Material()
	{
	}

	void Material::setProgram(Application::GLProgram * glProgram)
	{
		m_program = glProgram;
	}

	Application::GLProgram * Material::getProgram()
	{
		return m_program;
	}

}