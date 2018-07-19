#include "Texture.h"

#include <fstream>

namespace Application::RayTracer {

	Texture::Texture()
	{
	}

	Texture::~Texture()
	{
	}

	void Texture::create(unsigned int w, unsigned int h)
	{
		m_width = w;
		m_height = h;
		m_data.resize(w * h);
	}
	bool Texture::saveToFile(std::string path) const
	{
		// TODO use stb
		return false;
	}
	bool Texture::loadFromFile(std::string path)
	{
		// TODO use stb
		return false;
	}
	void Texture::set(unsigned int w, unsigned int h, Pixel pixel)
	{
		m_data[h * m_width + w] = pixel;
	}
	Pixel Texture::get(unsigned int w, unsigned int h) const
	{
		return m_data[h * m_width + w];
	}
	const float * Texture::data() const
	{
		return m_data.data()->data;
	}
	unsigned int Texture::width() const
	{
		return m_width;
	}
	unsigned int Texture::height() const
	{
		return m_height;
	}
	void Pixel::clamp()
	{
	}
	void Pixel::applyGamma()
	{
		// TODO pow 2.2f
	}
	Color32 Pixel::color32()
	{
		return Color32(r * 255, g * 255, b * 255, a * 255);
	}
	ColorHDR Pixel::colorHDR()
	{
		return ColorHDR(r, g, b, a);
	}
	Pixel::Pixel(const Color32 & color) :
		r(static_cast<float>(color.x) / 255.f),
		g(static_cast<float>(color.y) / 255.f),
		b(static_cast<float>(color.z) / 255.f),
		a(static_cast<float>(color.w) / 255.f)
	{
	}
	Pixel::Pixel(const ColorHDR & color) :
		r(color.x),
		g(color.y),
		b(color.z),
		a(color.w)
	{
	}
	Pixel::Pixel()
	{
	}
}