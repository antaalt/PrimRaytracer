#include "uv2.h"

namespace geometry {

	inline uv2::uv2()
	{
	}
	inline uv2::uv2(math::real_t value) : u(value), v(value)
	{
	}
	inline uv2::uv2(math::real_t u, math::real_t v) : u(u), v(v)
	{
	}
	inline math::real_t & uv2::operator[](size_t index)
	{
		return data[index];
	}
	inline bool operator==(const uv2 &lhs, const uv2 &rhs)
	{
		return lhs.u == rhs.u && lhs.v == rhs.v;
	}
	inline bool operator!=(const uv2 &lhs, const uv2 &rhs)
	{
		return !(lhs == rhs);
	}
	inline uv2 operator*(const uv2 &lhs, math::real_t rhs)
	{
		return uv2(lhs.u * rhs, lhs.v * rhs);
	}
	inline uv2 operator*(math::real_t lhs, const uv2 &rhs)
	{
		return uv2(lhs * rhs.u, lhs * rhs.v);
	}
	inline uv2 operator/(const uv2 &lhs, math::real_t rhs)
	{
		return uv2(lhs.u / rhs, lhs.v / rhs);
	}
	inline uv2 operator+(const uv2 &lhs, const uv2 &rhs)
	{
		return uv2(lhs.u + rhs.u, lhs.v + rhs.v);
	}
	inline uv2 operator-(const uv2 &lhs, const uv2 &rhs)
	{
		return uv2(lhs.u - rhs.u, lhs.v - rhs.v);
	}
	inline uv2 operator-(const uv2 &vec)
	{
		return uv2(-vec.u, -vec.v);
	}
}