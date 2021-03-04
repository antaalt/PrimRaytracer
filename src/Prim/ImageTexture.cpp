#include "ImageTexture.h"

#include <stb_image.h>

#define BILINEAR_FILTER_TEXTURE

namespace prim {

template<>
ImageTexture<color4f>::ImageTexture(const std::string & file)
{
	// Load file from file
	int w, h, comp;
	stbi_set_flip_vertically_on_load(true);
	stbi_uc * data = stbi_load(file.c_str(), &w, &h, &comp, 4);
	if (data == nullptr)
		throw std::runtime_error("Could not load file " + file);
	// Convert from byte to float
	m_data.resize(w * h);
	m_width = w;
	m_height = h;
	stbi_uc *src = data;
	color4f *dst = m_data.data();
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x, src += 4, dst++) {
			dst->data[0] = src[0] / 255.f;
			dst->data[1] = src[1] / 255.f;
			dst->data[2] = src[2] / 255.f;
			dst->data[3] = src[3] / 255.f;
		}
	}
	stbi_image_free(data);
}

template<>
ImageTexture<color32>::ImageTexture(const std::string & file)
{
	// Load file from file
	int w, h, comp;
	stbi_set_flip_vertically_on_load(true);
	stbi_uc * data = stbi_load(file.c_str(), &w, &h, &comp, 4);
	if (data == nullptr)
		throw std::runtime_error("Could not load file " + file);
	m_data = std::vector<color32>(data, data + w * h);
	m_width = w;
	m_height = h;
	stbi_image_free(data);
}

template<>
ImageTexture<float>::ImageTexture(const std::string & file)
{
	throw std::runtime_error("Not implemented");
}

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