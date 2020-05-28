#include "TextureMap.h"


namespace raycore {

template <typename T, typename U>
TTextureMap<T, U>::TTextureMap(const std::vector<U> &bytes, unsigned int width, unsigned int height) :
	m_data(bytes),
	m_width(width),
	m_height(height),
	m_components(4)
{
}

template<typename T, typename U>
TTextureMap<T, U>::TTextureMap(const U * data, unsigned int width, unsigned int height) :
	m_data(data, data + width * height * 4),
	m_width(width),
	m_height(height),
	m_components(4)
{	
}

template <typename T, typename U>
geometry::color4<T> TTextureMap<T, U>::evaluate(const geometry::uv2f &uv) const
{
	float ui = uv.u * m_width;
	float vi = uv.v * m_height;
	unsigned int uPixel = static_cast<unsigned int>(ui) % m_width;
	unsigned int vPixel = static_cast<unsigned int>(vi) % m_height;
#if defined(BILINEAR_FILTER_TEXTURE)
	float uf = ui - geometry::floor(ui);
	float vf = vi - geometry::floor(vi);
	return geometry::lerp<geometry::color4<T>>(
		geometry::lerp<geometry::color4<T>>(
			at(uPixel, vPixel),
			at(uPixel + 1, vPixel),
			uf
		),
		geometry::lerp<geometry::color4<T>>(
			at(uPixel, vPixel + 1),
			at(uPixel + 1, vPixel + 1),
			uf
		),
		vf
	);
#else
	unsigned int index = vPixel * m_width * m_component + uPixel * m_component;
	return color4(&this->data[index]);
#endif
}

template <typename T, typename U>
geometry::color4<T> TTextureMap<T, U>::at(unsigned int x, unsigned int y) const
{
#if defined(TEXTURE_REPEAT)
	x = x % m_width;
	y = y % m_height;
#else
	if (x >= m_width || x < 0 || y >= m_height || m_height == 0)
		return geometry::color4<T>(0.f);
#endif
	unsigned int index = y * m_width * m_components + x * m_components;
	return color4<T>(m_data[index]); // TODO conversion srgb ?
}

template <typename T, typename U>
unsigned int TTextureMap<T, U>::stride() const
{
	// TODO T or U ?
	return sizeof(T) * m_components;
}

template<typename T, typename U>
Texture<T> * TTextureMap<T, U>::clone() const
{
	return new TTextureMap<T, U>(*this);
}

template class TTextureMap<float, unsigned char>;
template class TTextureMap<float, float>;

}