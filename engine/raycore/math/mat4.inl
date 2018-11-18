#include "mat4.h"
#include "norm3.h"
#include "point3.h"
#include "vec3.h"
#include "quat.h"

namespace geometry {
	inline mat4::col::col()
	{
	}
	inline mat4::col::col(math::real_t value) :
		x(value), y(value), z(value), w(value)
	{
	}
	inline mat4::col::col(math::real_t x, math::real_t y, math::real_t z, math::real_t w) :
		x(x), y(y), z(z), w(w)
	{
	}
	inline mat4::col::col(norm3 vec, math::real_t w) :
		x(vec.x), y(vec.y), z(vec.z), w(w)
	{
	}
	inline mat4::col::col(vec3 vec, math::real_t w) :
		x(vec.x), y(vec.y), z(vec.z), w(w)
	{
	}
	inline mat4::col::col(point3 vec, math::real_t w) :
		x(vec.x), y(vec.y), z(vec.z), w(w)
	{
	}
	inline math::real_t & mat4::col::operator[](size_t index)
	{
		return data[index];
	}
	inline const math::real_t & mat4::col::operator[](size_t index) const
	{
		return data[index];
	}
	inline mat4::mat4()
	{
	}
	inline mat4::mat4(math::real_t value) :
		cols{ col(value), col(value), col(value), col(value) }
	{
	}
	inline mat4::mat4(col x, col y, col z, col w) :
		cols{ x, y, z, w }
	{
	}
	inline mat4::mat4(const quat & quat)
	{
		math::real_t sqw = quat.w*quat.w;
		math::real_t sqx = quat.x*quat.x;
		math::real_t sqy = quat.y*quat.y;
		math::real_t sqz = quat.z*quat.z;

		math::real_t invs = 1 / (sqx + sqy + sqz + sqw);
		cols[0] = col(
			(sqx - sqy - sqz + sqw)*invs,
			math::real_t(2) * (quat.x*quat.y + quat.z*quat.w)*invs,
			math::real_t(2) * (quat.x*quat.z - quat.y*quat.w)*invs,
			math::real_t(0)
		);
		cols[1] = col(
			math::real_t(2) * (quat.x*quat.y - quat.z*quat.w)*invs,
			(-sqx + sqy - sqz + sqw)*invs,
			math::real_t(2) * (quat.y*quat.z + quat.x*quat.w)*invs,
			math::real_t(0)
		);
		cols[2] = col(
			math::real_t(2) * (quat.x*quat.z + quat.y*quat.w)*invs,
			math::real_t(2) * (quat.y*quat.z - quat.x*quat.w)*invs,
			(-sqx - sqy + sqz + sqw)*invs,
			math::real_t(0)
		);
		cols[3] = col(
			math::real_t(0),
			math::real_t(0),
			math::real_t(0),
			math::real_t(1)
		);
	}
	inline mat4::col & mat4::operator[](size_t index)
	{
		return cols[index];
	}
	inline const mat4::col & mat4::operator[](size_t index) const
	{
		return cols[index];
	}
	inline mat4 operator*(const mat4& lhs, const mat4 &rhs)
	{
		mat4 out(0.f);
		for (int iCol = 0; iCol < 4; iCol++)
			for (int iRow = 0; iRow < 4; iRow++)
				for (int k = 0; k < 4; k++)
					out[iCol][iRow] += rhs[iCol][k] * lhs[k][iRow];
		return out;
	}
	inline point3 operator*(const mat4& lhs, const point3 &rhs)
	{
		return point3(
			lhs[0].x * rhs.x + lhs[1].x * rhs.y + lhs[2].x * rhs.z + lhs[3].x,
			lhs[0].y * rhs.x + lhs[1].y * rhs.y + lhs[2].y * rhs.z + lhs[3].y,
			lhs[0].z * rhs.x + lhs[1].z * rhs.y + lhs[2].z * rhs.z + lhs[3].z
		);
	}
	inline vec3 operator*(const mat4& lhs, const vec3 &rhs)
	{
		return vec3(
			lhs[0].x * rhs.x + lhs[1].x * rhs.y + lhs[2].x * rhs.z,
			lhs[0].y * rhs.x + lhs[1].y * rhs.y + lhs[2].y * rhs.z,
			lhs[0].z * rhs.x + lhs[1].z * rhs.y + lhs[2].z * rhs.z
		);
	}
	inline mat4 mat4::identity()
	{
		return mat4(
			mat4::col(1.f, 0.f, 0.f, 0.f),
			mat4::col(0.f, 1.f, 0.f, 0.f),
			mat4::col(0.f, 0.f, 1.f, 0.f),
			mat4::col(0.f, 0.f, 0.f, 1.f)
		);
	}
	inline mat4 mat4::rotate(const vec3 &axis, math::Radian angle)
	{
		// NOTE: Element 0,1 is wrong in Foley and Van Dam, Pg 227!
		math::real_t sintheta = math::sin(angle);
		math::real_t costheta = math::cos(angle);
		vec3 an = normalize(axis);
		math::real_t ux = an.x;
		math::real_t uy = an.y;
		math::real_t uz = an.z;
		return mat4(
			mat4::col(
				ux*ux + costheta*(1 - ux*ux),
				ux*uy*(math::real_t(1) - costheta) - uz*sintheta,
				uz*ux*(math::real_t(1) - costheta) + uy*sintheta,
				math::real_t(0)
			),
			mat4::col(
				ux*uy*(math::real_t(1) - costheta) + uz*sintheta,
				uy*uy + costheta*(math::real_t(1) - uy*uy),
				uy*uz*(math::real_t(1) - costheta) - ux*sintheta,
				math::real_t(0)
			),
			mat4::col(
				uz*ux*(math::real_t(1) - costheta) - uy*sintheta,
				uy*uz*(math::real_t(1) - costheta) + ux*sintheta,
				uz*uz + costheta*(math::real_t(1) - uz*uz),
				math::real_t(0)
			),
			mat4::col(math::real_t(0), math::real_t(0), math::real_t(0), math::real_t(1))
		);
	}
	inline mat4 mat4::translate(const vec3 &translation)
	{
		return mat4(
			mat4::col(math::real_t(1), math::real_t(0), math::real_t(0), math::real_t(0)),
			mat4::col(math::real_t(0), math::real_t(1), math::real_t(0), math::real_t(0)),
			mat4::col(math::real_t(0), math::real_t(0), math::real_t(1), math::real_t(0)),
			mat4::col(translation, math::real_t(1))
		);
	}
	inline mat4 mat4::scale(const vec3 &scale)
	{
		return mat4(
			mat4::col(scale.x, math::real_t(0), math::real_t(0), math::real_t(0)),
			mat4::col(math::real_t(0), scale.y, math::real_t(0), math::real_t(0)),
			mat4::col(math::real_t(0), math::real_t(0), scale.z, math::real_t(0)),
			mat4::col(math::real_t(0), math::real_t(0), math::real_t(0), math::real_t(1))
		);
	}
	inline mat4 TRS(const vec3 & t, const quat & r, const vec3 & s)
	{
		return mat4::translate(t) * mat4(r) * mat4::scale(s);
	}
	inline mat4 inverse(const mat4 &mat)
	{
		float A2323 = mat[2][2] * mat[3][3] - mat[2][3] * mat[3][2];
		float A1323 = mat[2][1] * mat[3][3] - mat[2][3] * mat[3][1];
		float A1223 = mat[2][1] * mat[3][2] - mat[2][2] * mat[3][1];
		float A0323 = mat[2][0] * mat[3][3] - mat[2][3] * mat[3][0];
		float A0223 = mat[2][0] * mat[3][2] - mat[2][2] * mat[3][0];
		float A0123 = mat[2][0] * mat[3][1] - mat[2][1] * mat[3][0];
		float A2313 = mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2];
		float A1313 = mat[1][1] * mat[3][3] - mat[1][3] * mat[3][1];
		float A1213 = mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1];
		float A2312 = mat[1][2] * mat[2][3] - mat[1][3] * mat[2][2];
		float A1312 = mat[1][1] * mat[2][3] - mat[1][3] * mat[2][1];
		float A1212 = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];
		float A0313 = mat[1][0] * mat[3][3] - mat[1][3] * mat[3][0];
		float A0213 = mat[1][0] * mat[3][2] - mat[1][2] * mat[3][0];
		float A0312 = mat[1][0] * mat[2][3] - mat[1][3] * mat[2][0];
		float A0212 = mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0];
		float A0113 = mat[1][0] * mat[3][1] - mat[1][1] * mat[3][0];
		float A0112 = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];

		float det = 1.f / mat.det();

		return mat4(
			mat4::col(
				det *   (mat[1][1] * A2323 - mat[1][2] * A1323 + mat[1][3] * A1223),
				det * -(mat[0][1] * A2323 - mat[0][2] * A1323 + mat[0][3] * A1223),
				det *   (mat[0][1] * A2313 - mat[0][2] * A1313 + mat[0][3] * A1213),
				det * -(mat[0][1] * A2312 - mat[0][2] * A1312 + mat[0][3] * A1212)
			),
			mat4::col(
				det * -(mat[1][0] * A2323 - mat[1][2] * A0323 + mat[1][3] * A0223),
				det *   (mat[0][0] * A2323 - mat[0][2] * A0323 + mat[0][3] * A0223),
				det * -(mat[0][0] * A2313 - mat[0][2] * A0313 + mat[0][3] * A0213),
				det *   (mat[0][0] * A2312 - mat[0][2] * A0312 + mat[0][3] * A0212)
			),
			mat4::col(
				det *   (mat[1][0] * A1323 - mat[1][1] * A0323 + mat[1][3] * A0123),
				det * -(mat[0][0] * A1323 - mat[0][1] * A0323 + mat[0][3] * A0123),
				det *   (mat[0][0] * A1313 - mat[0][1] * A0313 + mat[0][3] * A0113),
				det * -(mat[0][0] * A1312 - mat[0][1] * A0312 + mat[0][3] * A0112)
			),
			mat4::col(
				det * -(mat[1][0] * A1223 - mat[1][1] * A0223 + mat[1][2] * A0123),
				det *   (mat[0][0] * A1223 - mat[0][1] * A0223 + mat[0][2] * A0123),
				det * -(mat[0][0] * A1213 - mat[0][1] * A0213 + mat[0][2] * A0113),
				det *   (mat[0][0] * A1212 - mat[0][1] * A0212 + mat[0][2] * A0112)
			)
		);
	}
	inline float mat4::det() const
	{
		return
			cols[0][3] * cols[1][2] * cols[2][1] * cols[3][0] - cols[0][2] * cols[1][3] * cols[2][1] * cols[3][0] -
			cols[0][3] * cols[1][1] * cols[2][2] * cols[3][0] + cols[0][1] * cols[1][3] * cols[2][2] * cols[3][0] +
			cols[0][2] * cols[1][1] * cols[2][3] * cols[3][0] - cols[0][1] * cols[1][2] * cols[2][3] * cols[3][0] -
			cols[0][3] * cols[1][2] * cols[2][0] * cols[3][1] + cols[0][2] * cols[1][3] * cols[2][0] * cols[3][1] +
			cols[0][3] * cols[1][0] * cols[2][2] * cols[3][1] - cols[0][0] * cols[1][3] * cols[2][2] * cols[3][1] -
			cols[0][2] * cols[1][0] * cols[2][3] * cols[3][1] + cols[0][0] * cols[1][2] * cols[2][3] * cols[3][1] +
			cols[0][3] * cols[1][1] * cols[2][0] * cols[3][2] - cols[0][1] * cols[1][3] * cols[2][0] * cols[3][2] -
			cols[0][3] * cols[1][0] * cols[2][1] * cols[3][2] + cols[0][0] * cols[1][3] * cols[2][1] * cols[3][2] +
			cols[0][1] * cols[1][0] * cols[2][3] * cols[3][2] - cols[0][0] * cols[1][1] * cols[2][3] * cols[3][2] -
			cols[0][2] * cols[1][1] * cols[2][0] * cols[3][3] + cols[0][1] * cols[1][2] * cols[2][0] * cols[3][3] +
			cols[0][2] * cols[1][0] * cols[2][1] * cols[3][3] - cols[0][0] * cols[1][2] * cols[2][1] * cols[3][3] -
			cols[0][1] * cols[1][0] * cols[2][2] * cols[3][3] + cols[0][0] * cols[1][1] * cols[2][2] * cols[3][3];
	}
	inline mat4::col::operator point3()
	{
		return point3(x, y, z);
	}
	inline mat4::col::operator norm3()
	{
		return norm3(x, y, z);
	}
}