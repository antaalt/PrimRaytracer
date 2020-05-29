#include "mat3.h"
#include "norm3.h"
#include "point3.h"
#include "vec3.h"
#include "quat.h"
#include "mat4.h"

namespace geometry {

template <typename T>
inline col3<T>::col3()
{
}

template <typename T>
inline col3<T>::col3(T value) : x(value), y(value), z(value)
{
}

template <typename T>
inline col3<T>::col3(T x, T y, T z) : x(x), y(y), z(z)
{
}

template <typename T>
inline T & col3<T>::operator[](size_t index)
{
	return data[index];
}

template <typename T>
inline const T &col3<T>::operator[](size_t index) const
{
	return data[index];
}

template <typename T>
inline mat3<T>::mat3()
{
}

template <typename T>
inline mat3<T>::mat3(T value) : cols{ col(value), col(value), col(value) }
{
}

template <typename T>
inline mat3<T>::mat3(const mat4<T> & mat) : cols{
	col(mat[0].x, mat[0].y, mat[0].z),
	col(mat[1].x, mat[1].y, mat[1].z),
	col(mat[2].x, mat[2].y, mat[2].z)
}
{
}

template <typename T>
inline mat3<T>::mat3(col3<T> x, col3<T> y, col3<T> z) : cols{ x, y, z }
{
}

template <typename T>
inline col3<T> & mat3<T>::operator[](size_t index)
{
	return cols[index];
}

template <typename T>
inline const col3<T> & mat3<T>::operator[](size_t index) const
{
	return cols[index];
}

template <typename T>
inline mat3<T> mat3<T>::identity()
{
	return mat3(
		col3<T>(1.f, 0.f, 0.f),
		col3<T>(0.f, 1.f, 0.f),
		col3<T>(0.f, 0.f, 1.f)
	);
}

template <typename T>
inline mat3<T> operator*(const mat3<T>& lhs, const mat3<T> &rhs)
{
	mat3<T> out(0.f);
	for (int iCol = 0; iCol < 3; iCol++)
		for (int iRow = 0; iRow < 3; iRow++)
			for (int k = 0; k < 3; k++)
				out[iCol][iRow] += rhs[iCol][k] * lhs[k][iRow];
	return out;
}

template <typename T>
inline norm3<T> operator*(const mat3<T>& lhs, const norm3<T> &rhs)
{
	return norm3<T>(
		lhs[0].x * rhs.x + lhs[1].x * rhs.y + lhs[2].x * rhs.z + lhs[3].x,
		lhs[0].y * rhs.x + lhs[1].y * rhs.y + lhs[2].y * rhs.z + lhs[3].y,
		lhs[0].z * rhs.x + lhs[1].z * rhs.y + lhs[2].z * rhs.z + lhs[3].z
	);
}

}