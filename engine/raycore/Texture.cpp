#include "Texture.h"
#include "Config.h"
#include "Mathematic.h"

namespace raycore {

	template <typename T>
	TTexture<T>::TTexture(const std::vector<T> &bytes, unsigned int width, unsigned int height, unsigned int components) :
		width(width),
		height(height),
		components(components),
		data(bytes)
	{
	}

	template <typename T>
	colorHDR TTexture<T>::texture2D(float u, float v)
	{
		float ui = u * this->width;
		float vi = v * this->height;
		unsigned int uPixel = static_cast<unsigned int>(ui) % this->width;
		unsigned int vPixel = static_cast<unsigned int>(vi) % this->height;
#if defined(BILINEAR_FILTER_TEXTURE)
		float uf = ui - floorf(ui);
		float vf = vi - floorf(vi);
		return lerp(
			lerp(
				at(uPixel, vPixel),
				at(uPixel + 1, vPixel),
				uf
			),
			lerp(
				at(uPixel, vPixel + 1),
				at(uPixel + 1, vPixel + 1),
				uf
			),
			vf
		);
#else
		unsigned int index = vPixel * this->width * this->component + uPixel * this->component;
		return color4(&this->data[index]);
#endif
	}

	template <typename T>
	unsigned int TTexture<T>::stride()
	{
		return sizeof(float) * this->components;
	}

	template <typename T>
	colorHDR TTexture<T>::at(unsigned int x, unsigned int y)
	{
		return color(0.f);
	}
	
	template <>
	colorHDR TTexture<float>::at(unsigned int x, unsigned int y)
	{
#if defined(TEXTURE_REPEAT)
		x = x % this->width;
		y = y % this->height;
#else
		if (x >= this->width || x < 0 || y >= this->height || this->height == 0)
			return color4(0.f);
#endif
		unsigned int index = y * this->width * this->components + x * this->components;
		return colorHDR(
			this->data[index + 0],
			this->data[index + 1],
			this->data[index + 2],
			(this->components == 4) ? this->data[index + 3] : 1.f
		);
	}

	template <>
	colorHDR TTexture<unsigned char>::at(unsigned int x, unsigned int y)
	{
#if defined(TEXTURE_REPEAT)
		x = x % this->width;
		y = y % this->height;
#else
		if (x >= this->width || x < 0 || y >= this->height || this->height == 0)
			return color4(0.f);
#endif
		unsigned int index = y * this->width * this->components + x * this->components;
		return ldr2hdr(color32(
			this->data[index + 0],
			this->data[index + 1],
			this->data[index + 2],
			(this->components == 4) ? this->data[index + 3] : 255
		));
	}

	template struct TTexture<float>;
	template struct TTexture<unsigned char>;

}
