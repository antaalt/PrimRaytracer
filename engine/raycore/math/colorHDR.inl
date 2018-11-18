#include "colorHDR.h"
#include "color32.h"
#include "scientific.h"

namespace geometry {
	inline colorHDR::colorHDR()
	{
	}
	inline colorHDR::colorHDR(math::real_t value) :
		r(value), g(value), b(value), a(value)
	{
	}
	inline colorHDR::colorHDR(const color32 & color) :
		r(math::pow(color.r / 255.f, 2.2f)),
		g(math::pow(color.g / 255.f, 2.2f)),
		b(math::pow(color.b / 255.f, 2.2f)),
		a(color.a / 255.f)
	{
	}
	inline math::real_t & colorHDR::operator[](size_t index)
	{
		return data[index];
	}
	inline colorHDR::colorHDR(math::real_t r, math::real_t g, math::real_t b, math::real_t a) :
		r(r), g(g), b(b), a(a)
	{
	}
	inline bool operator==(const colorHDR &lhs, const colorHDR &rhs)
	{
		return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
	}
	inline bool operator!=(const colorHDR &lhs, const colorHDR &rhs)
	{
		return !(lhs == rhs);
	}
	inline colorHDR operator*(const colorHDR &lhs, const colorHDR &rhs)
	{
		return colorHDR(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b, lhs.a * rhs.a);
	}
	inline colorHDR operator*(const colorHDR &lhs, math::real_t rhs)
	{
		return colorHDR(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs);
	}
	inline colorHDR operator*(math::real_t rhs, const colorHDR &lhs)
	{
		return colorHDR(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs);
	}
	inline colorHDR operator/(const colorHDR &lhs, math::real_t rhs)
	{
		return colorHDR(lhs.r / rhs, lhs.g / rhs, lhs.b / rhs, lhs.a / rhs);
	}
	inline colorHDR operator+(const colorHDR &lhs, const colorHDR &rhs)
	{
		return colorHDR(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a);
	}
	inline colorHDR operator-(const colorHDR &lhs, const colorHDR &rhs)
	{
		return colorHDR(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a);
	}
	inline colorHDR operator-(const colorHDR &vec)
	{
		return colorHDR(-vec.r, -vec.g, -vec.b, -vec.a);
	}
	inline colorHDR &operator+=(colorHDR &lhs, const colorHDR &rhs)
	{
		lhs.r += rhs.r; lhs.g += rhs.g; lhs.b += rhs.b; lhs.a += rhs.a; return lhs;
	}
	inline colorHDR &operator-=(colorHDR &lhs, const colorHDR &rhs)
	{
		lhs.r -= rhs.r; lhs.g -= rhs.g; lhs.b -= rhs.b; lhs.a -= rhs.a; return lhs;
	}
	inline colorHDR saturate(const colorHDR &vec)
	{
		return colorHDR(
			math::saturate(vec.r), 
			math::saturate(vec.g), 
			math::saturate(vec.b), 
			math::saturate(vec.a)
		);
	}
	inline bool hasNan(const colorHDR & color)
	{
		return math::isNan(color.r) || math::isNan(color.g) || math::isNan(color.b) || math::isNan(color.a);
	}

}