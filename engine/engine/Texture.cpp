#include "Texture.h"


namespace Application {
	namespace GL {

		template<typename T>
		Texture<T>::Texture()
		{
		}

		template<typename T>
		Texture<T>::Texture(const unsigned char *data, unsigned int width, unsigned int height, Depth depth)
		{
			ASSERT(depth == RGB, "Not rgb texture");
			m_depth = depth;
			m_width = width;
			m_height = height;
			const int size = width * height * stride();
			m_data = std::vector<unsigned char>(data, data + size);
			// TODO check if we need to store image
			glGenTextures(1, &m_textureID);
			glBindTexture(GL_TEXTURE_2D, m_textureID);
			const GLenum type = (sizeof(T) == 4) ? GL_FLOAT : GL_UNSIGNED_BYTE;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, type, &m_data[0]);

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