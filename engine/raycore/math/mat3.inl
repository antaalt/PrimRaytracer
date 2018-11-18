#include "mat3.h"
#include "norm3.h"
#include "point3.h"
#include "vec3.h"
#include "quat.h"
#include "mat4.h"

namespace geometry {
	inline mat3::col::col()
	{
	}
	inline mat3::col::col(math::real_t value) : x(value), y(value), z(value)
	{
	}
	inline mat3::col::col(math::real_t x, math::real_t y, math::real_t z) : x(x), y(y), z(z)
	{
	}
	inline math::real_t & mat3::col::operator[](size_t index)
	{
		return data[index];
	}
	inline const math::real_t &mat3::col::operator[](size_t index) const
	{
		return data[index];
	}
	inline mat3::col::operator point3()
	{
		return point3(x, y, z);
	}
	inline mat3::col::operator norm3()
	{
		return norm3(x, y, z);
	}
	// --- MAT3
	inline mat3::mat3()
	{
	}
	inline mat3::mat3(math::real_t value) : cols{ col(value), col(value), col(value) }
	{
	}
	inline mat3::mat3(const mat4 & mat) : cols{
		col(mat[0].x, mat[0].y, mat[0].z),
		col(mat[1].x, mat[1].y, mat[1].z),
		col(mat[2].x, mat[2].y, mat[2].z)
	}
	{
	}
	inline mat3::mat3(col x, col y, col z) : cols{ x, y, z }
	{
	}
	inline mat3::col & mat3::operator[](size_t index)
	{
		return cols[index];
	}
	inline const mat3::col & mat3::operator[](size_t index) const
	{
		return cols[index];
	}
	inline mat3 mat3::identity()
	{
		return mat3(
			mat3::col(1.f, 0.f, 0.f),
			mat3::col(0.f, 1.f, 0.f),
			mat3::col(0.f, 0.f, 1.f)
		);
	}
	inline mat3 operator*(const mat3& lhs, const mat3 &rhs)
	{
		mat3 out(0.f);
		for (int iCol = 0; iCol < 3; iCol++)
			for (int iRow = 0; iRow < 3; iRow++)
				for (int k = 0; k < 3; k++)
					out[iCol][iRow] += rhs[iCol][k] * lhs[k][iRow];
		return out;
	}
	inline norm3 operator*(const mat3& lhs, const norm3 &rhs)
	{
		return norm3(
			lhs[0].x * rhs.x + lhs[1].x * rhs.y + lhs[2].x * rhs.z + lhs[3].x,
			lhs[0].y * rhs.x + lhs[1].y * rhs.y + lhs[2].y * rhs.z + lhs[3].y,
			lhs[0].z * rhs.x + lhs[1].z * rhs.y + lhs[2].z * rhs.z + lhs[3].z
		);
	}
}