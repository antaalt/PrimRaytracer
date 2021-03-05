#include "ImageTexture.h"

#include <stb_image.h>

#define BILINEAR_FILTER_TEXTURE

namespace prim {

template<typename T>
ImageTexture<T>::ImageTexture(const T * data, uint32_t width, uint32_t height) :
	m_data(data, data + width * height),
	m_width(width),
	m_height(height)
{
}

template <typename T>
T ImageTexture<T>::evaluate(const uv2f &uv) const
{
	float ui = uv.u * m_width;
	float vi = uv.v * m_height;
	uint32_t uPixel = static_cast<uint32_t>(ui) % m_width;
	uint32_t vPixel = static_cast<uint32_t>(vi) % m_height;
#if defined(BILINEAR_FILTER_TEXTURE)
	float uf = ui - floor(ui);
	float vf = vi - floor(vi);
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
	return at(uPixel, vPixel);
#endif
}

template <typename T>
T ImageTexture<T>::at(uint32_t x, uint32_t y) const
{
	x = x % m_width;
	y = y % m_height;
	return m_data[y * m_width + x];
}

template class ImageTexture<color4f>;
template class ImageTexture<color32>;
template class ImageTexture<float>;

}