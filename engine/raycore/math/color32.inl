#include "color32.h"
#include "colorHDR.h"

namespace geometry {
	
	inline color32::color32()
	{
	}
	inline color32::color32(math::byte_t value) :
		r(value), g(value), b(value), a(value)
	{
	}
	inline color32::color32(math::byte_t r, math::byte_t g, math::byte_t b, math::byte_t a) :
		r(r), g(g), b(b), a(a)
	{
	}
	inline color32::color32(const colorHDR & color) :
		r(static_cast<math::byte_t>(math::clamp(math::pow(color.r, 1 / 2.2f), 0.f, 1.f) * 255.f)),
		g(static_cast<math::byte_t>(math::clamp(math::pow(color.g, 1 / 2.2f), 0.f, 1.f) * 255.f)),
		b(static_cast<math::byte_t>(math::clamp(math::pow(color.b, 1 / 2.2f), 0.f, 1.f) * 255.f)),
		a(static_cast<math::byte_t>(math::clamp(color.a, 0.f, 1.f)* 255.f))
	{
	}
	inline math::byte_t & color32::operator[](size_t index)
	{
		return data[index];
	}
}