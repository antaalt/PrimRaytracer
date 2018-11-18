#include "point3.h"
#include "vec3.h"
#include "norm3.h"

namespace geometry {
	inline norm3::norm3()
	{
	}
	inline norm3::norm3(math::real_t value) : x(value), y(value), z(value)
	{
	}
	inline norm3::norm3(math::real_t x, math::real_t y, math::real_t z) :
		x(x), y(y), z(z)
	{
	}
	inline norm3::norm3(const vec3 & vec) :
		x(vec.x), y(vec.y), z(vec.z)
	{
	}
	inline norm3::norm3(const point3 & point) : x(point.x), y(point.y), z(point.z)
	{
	}
	inline math::real_t & norm3::operator[](size_t index)
	{
		return data[index];
	}
	inline bool operator==(const norm3 &lhs, const norm3 &rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
	}
	inline bool operator!=(const norm3 &lhs, const norm3 &rhs)
	{
		return !(lhs == rhs);
	}
	inline norm3 operator*(const norm3 &lhs, math::real_t rhs)
	{
		return norm3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
	}
	inline norm3 operator*(math::real_t lhs, const norm3 &rhs)
	{
		return norm3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs);
	}
	inline norm3 operator/(const norm3 &lhs, math::real_t rhs)
	{
		return norm3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
	}
	inline norm3 operator+(const norm3 &lhs, const norm3 &rhs)
	{
		return norm3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}
	inline norm3 operator-(const norm3 &lhs, const norm3 &rhs)
	{
		return norm3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}
	inline norm3 operator-(const norm3 &vec)
	{
		return norm3(-vec.x, -vec.y, -vec.z);
	}
	inline math::real_t dot(const norm3 &lhs, const norm3 &rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}
	inline norm3 normalize(const norm3 &vec)
	{
		return vec / math::sqrt(dot(vec, vec));
	}
	inline norm3 faceForward(const norm3 &n, const vec3 &v)
	{
		return (dot(v, n) < 0.f) ? -n : n;
	}
}