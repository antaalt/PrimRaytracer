#include "TextureMap.h"


namespace raycore {

	template <typename T, typename U>
	TTextureMap<T, U>::TTextureMap(const std::vector<U> &bytes, unsigned int width, unsigned int height, unsigned int components) :
		width(width),
		height(height),
		components(components),
		data(bytes)
	{
	}

	template <typename T, typename U>
	T TTextureMap<T, U>::evaluate(const uv2 &uv = uv2(0.f)) const
	{
		float ui = uv.u * this->width;
		float vi = uv.v * this->height;
		unsigned int uPixel = static_cast<unsigned int>(ui) % this->width;
		unsigned int vPixel = static_cast<unsigned int>(vi) % this->height;
#if defined(BILINEAR_FILTER_TEXTURE)
		float uf = ui - floorf(ui);
		float vf = vi - floorf(vi);
		return lerp<T>(
			lerp<T>(
				at(uPixel, vPixel),
				at(uPixel + 1, vPixel),
				uf
			),
			lerp<T>(
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

	template <typename T, typename U>
	unsigned int TTextureMap<T, U>::stride() const
	{
		return sizeof(float) * this->components;
	}

	template <typename T, typename U>
	T TTextureMap<T, U>::at(unsigned int x, unsigned int y) const
	{
		return 0.f;
	}

	template<typename T, typename U>
	Texture<T> * TTextureMap<T, U>::clone() const
	{
		return new TTextureMap<T, U>(*this);
	}

	template <>
	float TTextureMap<float, unsigned char>::at(unsigned int x, unsigned int y) const
	{
#if defined(TEXTURE_REPEAT)
		x = x % this->width;
		y = y % this->height;
#else
		if (x >= this->width || x < 0 || y >= this->height || this->height == 0)
			return color4(0.f);
#endif
		unsigned int index = y * this->width * this->components + x * this->components;
		return powf(this->data[index], 2.2f);
	}

	template <>
	float TTextureMap<float, float>::at(unsigned int x, unsigned int y) const
	{
#if defined(TEXTURE_REPEAT)
		x = x % this->width;
		y = y % this->height;
#else
		if (x >= this->width || x < 0 || y >= this->height || this->height == 0)
			return color4(0.f);
#endif
		unsigned int index = y * this->width * this->components + x * this->components;
		return this->data[index];
	}

	template <>
	colorHDR TTextureMap<colorHDR, float>::at(unsigned int x, unsigned int y) const
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
	colorHDR TTextureMap<colorHDR, unsigned char>::at(unsigned int x, unsigned int y) const
	{
#if defined(TEXTURE_REPEAT)
		x = x % this->width;
		y = y % this->height;
#else
		if (x >= this->width || x < 0 || y >= this->height || this->height == 0)
			return color4(0.f);
#endif
		unsigned int index = y * this->width * this->components + x * this->components;
		return colorHDR(color32(
			this->data[index + 0],
			this->data[index + 1],
			this->data[index + 2],
			(this->components == 4) ? this->data[index + 3] : 255
		));
	}

	template class TTextureMap<float, unsigned char>;
	template class TTextureMap<float, float>;
	template class TTextureMap<colorHDR, float>;
	template class TTextureMap<colorHDR, unsigned char>;

}