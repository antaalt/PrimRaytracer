#include "PixelBuffer.h"

namespace app {
	void Pixel::clamp()
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			if (data[i] > 1.f) data[i] = 1.f;
			if (data[i] < 0.f) data[i] = 0.f;
		}
	}
	void Pixel::applyGamma()
	{
	}
	Pixel::operator Color32()
	{
		return Color32(
			static_cast<unsigned char>(r),
			static_cast<unsigned char>(g),
			static_cast<unsigned char>(b),
			static_cast<unsigned char>(a)
		);
	}
	Pixel::operator ColorHDR()
	{
		return ColorHDR(r, g, b, a);
	}
	/*void Pixel::operator=(const Color32 & color)
	{
		r = (color.x / 255.f);
		g = (color.y / 255.f);
		b = (color.z / 255.f);
		a = (color.w / 255.f);
	}
	void Pixel::operator=(const ColorHDR & color)
	{
		r = color.x;
		g = color.y;
		b = color.z;
		a = color.w;
	}*/
	Pixel Pixel::operator+(const Pixel & rhs)
	{
		return Pixel(
			r + rhs.r,
			g + rhs.g,
			b + rhs.b,
			a + rhs.a
		);
	}
	Pixel::Pixel()
	{
	}
	Pixel::Pixel(float val) : r(val), g(val), b(val), a(val)
	{
	}
	Pixel::Pixel(const Color32 & color)
	{
		r = (color.x / 255.f);
		g = (color.y / 255.f);
		b = (color.z / 255.f);
		a = (color.w / 255.f);
	}
	Pixel::Pixel(const ColorHDR & color)
	{
		r = color.x;
		g = color.y;
		b = color.z;
		a = color.w;
	}
	Pixel::Pixel(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
	{
	}
	PixelBuffer::PixelBuffer()
	{
	}
	PixelBuffer::PixelBuffer(unsigned int w, unsigned int h)
	{
		resize(w, h);
	}
	PixelBuffer::~PixelBuffer()
	{
	}
	void PixelBuffer::resize(unsigned int w, unsigned int h)
	{
		m_width = w;
		m_height = h;
		m_data.resize(w*h);
	}
	Pixel & PixelBuffer::operator()(unsigned int x, unsigned int y)
	{
		return m_data[y * m_width + x];
	}
	const Pixel & PixelBuffer::operator()(unsigned int x, unsigned int y) const
	{
		return m_data[y * m_width + x];
	}
	Pixel * PixelBuffer::data()
	{
		return m_data.data();
	}
	unsigned int PixelBuffer::width() const
	{
		return m_width;
	}
	unsigned int PixelBuffer::height() const
	{
		return m_height;
	}
	Pixel operator*(float lhs, const Pixel & rhs)
	{
		return Pixel(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b, lhs * rhs.a);
	}
	Pixel operator*(const Pixel & lhs, float rhs)
	{
		return Pixel(rhs * lhs.r, rhs * lhs.g, rhs * lhs.b, rhs * lhs.a);
	}
}