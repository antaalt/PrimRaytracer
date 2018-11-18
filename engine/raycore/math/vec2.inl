#include "vec2.h"

namespace geometry {
	inline vec2::vec2()
	{
	}
	inline vec2::vec2(math::real_t value) : x(value), y(value)
	{
	}
	inline vec2::vec2(math::real_t x, math::real_t y) : x(x), y(y)
	{
	}
	inline vec2::vec2(const vec3 &vec) : x(vec.x), y(vec.y)
	{
	}
	inline math::real_t &vec2::operator[](size_t index)
	{
		return data[index];
	}
	inline const math::real_t &vec2::operator[](size_t index) const
	{
		return data[index];
	}
	inline math::real_t dot(const vec2 &lhs, const vec2 &rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}
	inline math::real_t length(const vec2 &vec)
	{
		return math::sqrt(dot(vec, vec));
	}
}