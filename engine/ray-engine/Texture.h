#pragma once
#include <vector>
#include "Types.h"

#define BILINEAR_FILTER_TEXTURE
#define TEXTURE_REPEAT

namespace app {

	inline colorHDR ldr2hdr(const color32 &c)
	{
		return colorHDR(
			powf(c.x / 255.f, 2.2f),
			powf(c.y / 255.f, 2.2f),
			powf(c.z / 255.f, 2.2f),
			powf(c.w / 255.f, 2.2f)
		);
	}

	template <typename T>
	struct TTexture {
		TTexture(const std::vector<T> &data, unsigned int width, unsigned int height, unsigned int components);
		colorHDR texture2D(float u, float v);
		unsigned int stride();

		colorHDR at(unsigned int x, unsigned int y);
	private:
		std::vector<T> data;
		unsigned int width, height;
		unsigned int components;
	};

	using Texture = TTexture<float>;
	using Texture32 = TTexture<unsigned char>;
}