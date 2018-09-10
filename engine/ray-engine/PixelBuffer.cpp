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
	Pixel::operator color4()
	{
		return color4(r, g, b, a);
	}
	
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
	Pixel::Pixel(const color4 & color4)
	{
		r = color4.x;
		g = color4.y;
		b = color4.z;
		a = color4.w;
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
	const Pixel * PixelBuffer::data() const
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