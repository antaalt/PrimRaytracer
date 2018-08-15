#include "Texture.h"


namespace app {
	namespace GL {

		template<typename T>
		Texture<T>::Texture()
		{
		}

		template<typename T>
		Texture<T>::Texture(const std::vector<unsigned char> &data, unsigned int width, unsigned int height, unsigned int components)
		{
			m_depth = static_cast<Depth>(components);
			m_width = width;
			m_height = height;
			const int size = width * height * stride();
			if(m_depth == RGBA)
				m_data = data;
			else
			{
				ASSERT(m_depth == RGB, "Not correct");
				m_data.resize(m_width * m_height * 4);
				for (unsigned int y = 0; y < m_height; y++)
				{
					for (unsigned int x = 0; x < m_width; x++)
					{
						const unsigned int outIndex = y * m_width * 4 + x * 4;
						const unsigned int inIndex = y * m_width * 3 + x * 3;
						m_data[outIndex+0] = data[inIndex+0];
						m_data[outIndex+1] = data[inIndex+1];
						m_data[outIndex+2] = data[inIndex+2];
						m_data[outIndex+3] = (sizeof(T) == 4) ? 1.f : 255;
					}
				}
			}
			// TODO check if we need to store image
			glGenTextures(1, &m_textureID);
			glBindTexture(GL_TEXTURE_2D, m_textureID);
			const GLenum type = (sizeof(T) == 4) ? GL_FLOAT : GL_UNSIGNED_BYTE;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, type, &m_data[0]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

		template<typename T>
		Texture<T>::~Texture()
		{
			if (m_textureID > 0)
				glDeleteTextures(1, &m_textureID);
		}
		template<typename T>
		int Texture<T>::stride() const
		{
			return m_depth * sizeof(T);
		}

		template<typename T>
		void Texture<T>::use(int channel) const
		{
			glActiveTexture(GL_TEXTURE0 + channel);
			glBindTexture(GL_TEXTURE_2D, m_textureID);
		}

		template<typename T>
		void Texture<T>::doNotUse(int channel) const
		{
			glActiveTexture(GL_TEXTURE0 + channel);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		template<typename T>
		GLuint Texture<T>::getID()
		{
			return m_textureID;
		}

		template class Texture<float>;
		template class Texture<unsigned char>;
	}
}