#include "Material.h"

namespace app {

	Material::Material()
	{
	}


	Material::~Material()
	{
	}

	bool Material::use()
	{
		for(auto it = m_textures.begin(); it != m_textures.end(); it++)
		{
			GL::Texture32 *texture = it->second;
			texture->use(it->first);
		}
		if (m_program != nullptr)
		{
			m_program->use();
			if (m_program->isValid())
			{
				// TODO update the uniforms material

			}
		}
		return false;
	}

	GL::Program * Material::getProgram()
	{
		return m_program;
	}

	void Material::setProgram(GL::Program * program)
	{
		m_program = program;
	}

	void Material::setColor(const glm::vec4 & color)
	{
		m_color = color;
	}

	void Material::setTexture(TextureType textureType, GL::Texture32 * texture)
	{
		// TODO CHECK memory leak when replacing existing one
		m_textures.insert(std::make_pair(textureType, texture));
	}

	GL::Texture32 * Material::getTexture(TextureType textureType)
	{
		auto it = m_textures.find(textureType);
		if (it == m_textures.end())
			return nullptr;
		return it->second;
	}

}