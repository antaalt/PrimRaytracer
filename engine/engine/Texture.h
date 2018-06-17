#pragma once
#include "Image.h"
#include "Config.h"

namespace Application {
	namespace GL {

		enum Depth {
			RGB = 3,
			RGBA = 4
		};
		template <typename T>
		class Texture
		{
		public:
			Texture();
			Texture(const unsigned char *data, unsigned int width, unsigned int height, Depth depth);
			~Texture();

			int stride() const;

			void use(int channel) const;
			void doNotUse(int channel) const;

			GLuint getID();

		private:
			std::vector<unsigned char> m_data;
			unsigned int m_width, m_height;
			Depth m_depth;

			GLuint m_textureID;
		};

		typedef Texture<float> Texturef;
		typedef Texture<unsigned char> Texture32;
	}
}

