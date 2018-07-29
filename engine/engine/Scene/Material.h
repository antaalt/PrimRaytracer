#pragma once

#include "../Program.h"
#include "../Texture.h"

// PBR material
// @see https://www.allegorithmic.com/pbr-guide
namespace app {

	enum TextureType {
		COLOR_TEXTURE,
		NORMAL_TEXTURE,
		METALNESS_TEXTURE,
		ROUGHNESS_TEXTURE,
		NB_TEXTURES
	};

	class Material
	{
	public:
		Material();
		~Material();

		bool use();

		GL::Program * getProgram();
		void setProgram(GL::Program *program);
		void setColor(const glm::vec4 &color);

		void setTexture(TextureType textureType, GL::Texture32 *texture);
		GL::Texture32 *getTexture(TextureType textureType);

	private:
		glm::vec4 m_color;
		GL::Program *m_program;
		std::map<TextureType, GL::Texture32*> m_textures;
	};

}