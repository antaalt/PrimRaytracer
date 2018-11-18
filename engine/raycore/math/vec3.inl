#include "vec3.h"
#include "norm3.h"
#include "point3.h"
#include "vec2.h"

namespace geometry {
	inline vec3::vec3()
	{
	}
	inline vec3::vec3(math::real_t value) :
		x(value), y(value), z(value)
	{
	}
	inline vec3::vec3(math::real_t x, math::real_t y, math::real_t z) : 
		x(x), y(y), z(z)
	{
	}
	inline vec3::vec3(const norm3 &normal) : 
		x(normal.x), y(normal.y), z(normal.z)
	{
	}
	inline vec3::vec3(const point3 &point) : 
		x(point.x), y(point.y), z(point.z)
	{
	}
	inline vec3::vec3(const vec2 &vec, math::real_t z) : 
		x(vec.x), y(vec.y), z(z)
	{
	}
	inline math::real_t & vec3::operator[](size_t index)
	{
		return data[index];
	}
	inline const math::real_t & vec3::operator[](size_t index) const
	{
		return data[index];
	}
	inline bool operator==(const vec3 & lhs, const vec3 & rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
	}
	inline bool operator!=(const vec3 &lhs, const vec3 &rhs)
	{
		return !(lhs == rhs);
	}
	inline vec3 operator*(const vec3 &lhs, math::real_t rhs)
	{
		return vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
	}
	inline vec3 operator*(math::real_t lhs, const vec3 &rhs)
	{
		return vec3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs);
	}
	inline vec3 operator/(const vec3 &lhs, math::real_t rhs)
	{
		return vec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
	}
	inline vec3 operator+(const vec3 &lhs, const vec3 &rhs)
	{
		return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}
	inline vec3 operator-(const vec3 &lhs, const vec3 &rhs)
	{
		return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}
	inline vec3 operator-(const vec3 &vec)
	{
		return vec3(-vec.x, -vec.y, -vec.z);
	}
	inline float dot(const vec3 & lhs, const vec3 & rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}
	inline vec3 cross(const vec3 & lhs, const vec3 & rhs)
	{
		return vec3(
			lhs.y * rhs.z - lhs.z * rhs.y, 
			lhs.z * rhs.x - lhs.x * rhs.z, 
			lhs.x * rhs.y - lhs.y * rhs.x
		);
	}
	inline math::real_t length(const vec3 &vec)
	{
		return math::sqrt(dot(vec, vec));
	}
	inline vec3 normalize(const vec3 &vec)
	{
		return vec / length(vec);
	}
}