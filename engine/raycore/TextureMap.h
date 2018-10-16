#pragma once
#include "Texture.h"

namespace raycore {

	template <typename T, typename U>
	class TTextureMap : public Texture<T> {
	public:
		TTextureMap(const std::vector<U> &data, unsigned int width, unsigned int height, unsigned int components = 1);
		virtual T evaluate(const uv2 &uv = uv2(0.f)) const;
		unsigned int stride() const;
		T at(unsigned int x, unsigned int y) const;

		virtual Texture* clone() const;
	private:
		std::vector<U> data;
		unsigned int width, height;
		unsigned int components;
	};

	using TextureMap = TTextureMap<colorHDR, float>;
	using TextureMap32 = TTextureMap<colorHDR, unsigned char>;
	using TextureMapFloat = TTextureMap<float, float>;
	using TextureMapFloat32 = TTextureMap<float, unsigned char>;
}

