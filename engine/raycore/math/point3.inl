#include "point3.h"
#include "vec3.h"
#include "norm3.h"

namespace geometry {
	inline point3::point3()
	{
	}
	inline point3::point3(math::real_t value) : x(value), y(value), z(value)
	{
	}
	inline point3::point3(math::real_t x, math::real_t y, math::real_t z) : x(x), y(y), z(z)
	{
	}
	inline point3::point3(const norm3 & normal) : x(normal.x), y(normal.y), z(normal.z)
	{
	}
	inline point3::point3(const vec3 & vec) : x(vec.x), y(vec.y), z(vec.z)
	{
	}
	inline math::real_t & point3::operator[](size_t index)
	{
		return data[index];
	}
	inline bool operator==(const point3 &lhs, const point3 &rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
	}
	inline bool operator!=(const point3 &lhs, const point3 &rhs)
	{
		return !(lhs == rhs);
	}
	inline point3 operator*(const point3 &lhs, math::real_t rhs)
	{
		return point3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
	}
	inline point3 operator*(math::real_t lhs, const point3 &rhs)
	{
		return point3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs);
	}
	inline point3 operator/(const point3 &lhs, math::real_t rhs)
	{
		return point3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
	}
	inline point3 operator+(const point3 &lhs, const point3 &rhs)
	{
		return point3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}
	inline point3 operator-(const point3 &lhs, const point3 &rhs)
	{
		return point3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}
	inline point3 operator-(const point3 &vec)
	{
		return point3(-vec.x, -vec.y, -vec.z);
	}
	inline point3 &operator+=(point3 &lhs, const point3 &rhs)
	{
		lhs.x += rhs.x; lhs.y += rhs.y; lhs.z += rhs.z; return lhs;
	}
	inline point3 &operator-=(point3 &lhs, const point3 &rhs)
	{
		lhs.x -= rhs.x; lhs.y -= rhs.y; lhs.z -= rhs.z; return lhs;
	}
	inline point3 &operator/=(point3 &lhs, float rhs)
	{
		lhs.x /= rhs; lhs.y /= rhs; lhs.z /= rhs; return lhs;
	}
	inline math::real_t distance(const point3 &lhs, const point3 &rhs)
	{
		return length(vec3(rhs - lhs));
	}
}