#pragma once
#include "Texture.h"

namespace raycore {

	template <typename T>
	class TTextureMap : public Texture {
	public:
		TTextureMap(const std::vector<T> &data, unsigned int width, unsigned int height, unsigned int components);
		virtual colorHDR texture2D(float u = 0.f, float v = 0.f) const;
		unsigned int stride() const;
		colorHDR at(unsigned int x, unsigned int y) const;

		virtual Texture* clone() const;
	private:
		std::vector<T> data;
		unsigned int width, height;
		unsigned int components;
	};

	using TextureMap = TTextureMap<float>;
	using TextureMap32 = TTextureMap<unsigned char>;
}

