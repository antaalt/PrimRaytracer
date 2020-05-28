#include "PixelBuffer.h"

namespace raycore {
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
	void Pixel::accumulate(const Pixel & pixel, unsigned int samples)
	{
		r = geometry::lerp(r, pixel.r, 1.f / (samples + 1.f));
		g = geometry::lerp(g, pixel.g, 1.f / (samples + 1.f));
		b = geometry::lerp(b, pixel.b, 1.f / (samples + 1.f));
		a = geometry::lerp(a, pixel.a, 1.f / (samples + 1.f));
	}
	Pixel::operator color4f()
	{
		return color4f(r, g, b, a);
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
	Pixel::Pixel(const color4f & color) : r(color.r), g(color.g), b(color.b), a(color.a)
	{
	}
	Pixel::Pixel(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
	{
	}
	PixelBuffer::PixelBuffer(unsigned int w, unsigned int h)
	{
		resize(w, h);
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
	unsigned int PixelBuffer::size() const
	{
		return m_width * m_height * sizeof(Pixel);
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