#pragma once

#include "Vector4.h"
#include "Vector3.h"
#include "Quaternion.h"

namespace Math {

	template <typename T>
	struct Matrix4 {
		using Col = Vector4<T>;
		union {
			T data[4 * 4];
			struct {
				T	xx, yx, zx, wx,
					xy, yy, zy, wy,
					xz, yz, zz, wz,
					xw, yw, zw, ww;
			};
			Vector4<T> col[4];
		};
		Matrix4();
		Matrix4(const Col &col1, const Col &col2, const Col &col3, const Col &col4);
		Matrix4(const T * data);

		inline Matrix4<T> transpose() const;
		inline Matrix4<T> inverse() const;
		static Matrix4<T> identity();
		static Matrix4<T> zero();

		inline float det() const;

		static Matrix4<T> TRS(const Vector3<T> &translation, const Quaternion<T> &rotation, const Vector3<T> &scale);

		template <typename T>
		friend Matrix4<T> operator*(const Matrix4<T> &lhs, const Matrix4<T> &rhs);

		template <typename T>
		friend Vector4<T> operator*(const Matrix4<T> &lhs, const Vector4<T> &rhs);
		template <typename T>
		friend Vector3<T> operator*(const Matrix4<T> &lhs, const Vector3<T> &rhs);

		inline bool operator==(const Matrix4<T> &c) const;
		inline bool operator!=(const Matrix4<T> &c) const;

		inline Col &operator[](const unsigned int col);
		inline const Col &operator[](const unsigned int col) const;
	};

	template<typename T>
	inline Matrix4<T> operator*(const Matrix4<T>& lhs, const Matrix4<T>& rhs)
	{
		Matrix4<T> out = Matrix4<T>::zero();
		for (int iCol = 0; iCol < 4; iCol++)
			for (int iRow = 0; iRow < 4; iRow++)
				for (int k = 0; k < 4; k++)
					out[iCol][iRow] += rhs[iCol][k] * lhs[k][iRow];
		return out;
	}

	template<typename T>
	inline Vector4<T> operator*(const Matrix4<T> &lhs, const Vector4<T> &rhs)
	{
		return Vector4<T>(
			lhs.col[0].x * rhs.x + lhs.col[1].x * rhs.y + lhs.col[2].x * rhs.z + lhs.col[3].x * rhs.w,
			lhs.col[0].y * rhs.x + lhs.col[1].y * rhs.y + lhs.col[2].y * rhs.z + lhs.col[3].y * rhs.w,
			lhs.col[0].z * rhs.x + lhs.col[1].z * rhs.y + lhs.col[2].z * rhs.z + lhs.col[3].z * rhs.w,
			lhs.col[0].w * rhs.x + lhs.col[1].w * rhs.y + lhs.col[2].w * rhs.z + lhs.col[3].w * rhs.w
			);
	}

	template<typename T>
	Vector3<T> operator*(const Matrix4<T>& lhs, const Vector3<T>& rhs)
	{
		return Vector3<T>(
			lhs.col[0].x * rhs.x + lhs.col[1].x * rhs.y + lhs.col[2].x * rhs.z + lhs.col[3].x,
			lhs.col[0].y * rhs.x + lhs.col[1].y * rhs.y + lhs.col[2].y * rhs.z + lhs.col[3].y,
			lhs.col[0].z * rhs.x + lhs.col[1].z * rhs.y + lhs.col[2].z * rhs.z + lhs.col[3].z
			);
	}

	template<typename T>
	inline Matrix4<T>::Matrix4()
	{
	}

	template<typename T>
	inline Matrix4<T>::Matrix4(const Col & col1, const Col & col2, const Col & col3, const Col & col4)
	{
		col[0] = col1;
		col[1] = col2;
		col[2] = col3;
		col[3] = col4;
	}

	template<typename T>
	inline Matrix4<T>::Matrix4(const T * data)
	{
		for (int iCol = 0; iCol < 4; iCol++)
			for (int iRow = 0; iRow < 4; iRow++)
				col[iCol][iRow] = data[iCol * 4 + iRow];
	}

	template<typename T>
	inline Matrix4<T> Matrix4<T>::transpose() const
	{
		Matrix4<T> out;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				out[j][i] = col[i][j];
		return out;
	}
	template<typename T>
	inline Matrix4<T> Matrix4<T>::inverse() const
	{
		float d =
			col[0][0] * col[1][1] * col[2][2] * col[3][3] + col[0][0] * col[1][2] * col[2][3] * col[3][1] + col[0][0] * col[1][3] * col[2][1] * col[3][2] +
			col[0][1] * col[1][0] * col[2][3] * col[3][2] + col[0][1] * col[1][2] * col[2][0] * col[3][3] + col[0][1] * col[1][3] * col[2][2] * col[3][0] +
			col[0][2] * col[1][0] * col[2][1] * col[3][3] + col[0][2] * col[1][1] * col[2][3] * col[3][0] + col[0][2] * col[1][3] * col[2][0] * col[3][1] +
			col[0][3] * col[1][0] * col[2][2] * col[3][1] + col[0][3] * col[1][1] * col[2][0] * col[3][2] + col[0][3] * col[1][2] * col[2][1] * col[3][0] -
			col[0][0] * col[1][1] * col[2][3] * col[3][2] - col[0][0] * col[1][2] * col[2][1] * col[3][3] - col[0][0] * col[1][3] * col[2][2] * col[3][1] -
			col[0][1] * col[1][0] * col[2][2] * col[3][3] - col[0][1] * col[1][2] * col[2][3] * col[3][0] - col[0][1] * col[1][3] * col[2][0] * col[3][2] -
			col[0][2] * col[1][0] * col[2][3] * col[3][1] - col[0][2] * col[1][1] * col[2][0] * col[3][3] - col[0][2] * col[1][3] * col[2][1] * col[3][0] -
			col[0][3] * col[1][0] * col[2][1] * col[3][2] - col[0][3] * col[1][1] * col[2][2] * col[3][0] - col[0][3] * col[1][2] * col[2][0] * col[3][1];

		if (d == 0.f)
		{
			std::cerr << "Cannot inverse matrix" << std::endl;
			return (*this);
		}
		d = 1.f / d;

		Matrix4<T> m;
		m[0][0] = col[1][1] * col[2][2] * col[3][3] * d + col[1][2] * col[2][3] * col[3][1] * d + col[1][3] * col[2][1] * col[3][2] * d - col[1][1] * col[2][3] * col[3][2] * d - col[1][2] * col[2][1] * col[3][3] * d - col[1][3] * col[2][2] * col[3][1] * d;
		m[0][1] = col[0][1] * col[2][3] * col[3][2] * d + col[0][2] * col[2][1] * col[3][3] * d + col[0][3] * col[2][2] * col[3][1] * d - col[0][1] * col[2][2] * col[3][3] * d - col[0][2] * col[2][3] * col[3][1] * d - col[0][3] * col[2][1] * col[3][2] * d;
		m[0][2] = col[0][1] * col[1][2] * col[3][3] * d + col[0][2] * col[1][3] * col[3][1] * d + col[0][3] * col[1][1] * col[3][2] * d - col[0][1] * col[1][3] * col[3][2] * d - col[0][2] * col[1][1] * col[3][3] * d - col[0][3] * col[1][2] * col[3][1] * d;
		m[0][3] = col[0][1] * col[1][3] * col[2][2] * d + col[0][2] * col[1][1] * col[2][3] * d + col[0][3] * col[1][2] * col[2][1] * d - col[0][1] * col[1][2] * col[2][3] * d - col[0][2] * col[1][3] * col[2][1] * d - col[0][3] * col[1][1] * col[2][2] * d;
		m[1][0] = col[1][0] * col[2][3] * col[3][2] * d + col[1][2] * col[2][0] * col[3][3] * d + col[1][3] * col[2][2] * col[3][0] * d - col[1][0] * col[2][2] * col[3][3] * d - col[1][2] * col[2][3] * col[3][0] * d - col[1][3] * col[2][0] * col[3][2] * d;
		m[1][1] = col[0][0] * col[2][2] * col[3][3] * d + col[0][2] * col[2][3] * col[3][0] * d + col[0][3] * col[2][0] * col[3][2] * d - col[0][0] * col[2][3] * col[3][2] * d - col[0][2] * col[2][0] * col[3][3] * d - col[0][3] * col[2][2] * col[3][0] * d;
		m[1][2] = col[0][0] * col[1][3] * col[3][2] * d + col[0][2] * col[1][0] * col[3][3] * d + col[0][3] * col[1][2] * col[3][0] * d - col[0][0] * col[1][2] * col[3][3] * d - col[0][2] * col[1][3] * col[3][0] * d - col[0][3] * col[1][0] * col[3][2] * d;
		m[1][3] = col[0][0] * col[1][2] * col[2][3] * d + col[0][2] * col[1][3] * col[2][0] * d + col[0][3] * col[1][0] * col[2][2] * d - col[0][0] * col[1][3] * col[2][2] * d - col[0][2] * col[1][0] * col[2][3] * d - col[0][3] * col[1][2] * col[2][0] * d;
		m[2][0] = col[1][0] * col[2][1] * col[3][3] * d + col[1][1] * col[2][3] * col[3][0] * d + col[1][3] * col[2][0] * col[3][1] * d - col[1][0] * col[2][3] * col[3][1] * d - col[1][1] * col[2][0] * col[3][3] * d - col[1][3] * col[2][1] * col[3][0] * d;
		m[2][1] = col[0][0] * col[2][3] * col[3][1] * d + col[0][1] * col[2][0] * col[3][3] * d + col[0][3] * col[2][1] * col[3][0] * d - col[0][0] * col[2][1] * col[3][3] * d - col[0][1] * col[2][3] * col[3][0] * d - col[0][3] * col[2][0] * col[3][1] * d;
		m[2][2] = col[0][0] * col[1][1] * col[3][3] * d + col[0][1] * col[1][3] * col[3][0] * d + col[0][3] * col[1][0] * col[3][1] * d - col[0][0] * col[1][3] * col[3][1] * d - col[0][1] * col[1][0] * col[3][3] * d - col[0][3] * col[1][1] * col[3][0] * d;
		m[2][3] = col[0][0] * col[1][3] * col[2][1] * d + col[0][1] * col[1][0] * col[2][3] * d + col[0][3] * col[1][1] * col[2][0] * d - col[0][0] * col[1][1] * col[2][3] * d - col[0][1] * col[1][3] * col[2][0] * d - col[0][3] * col[1][0] * col[2][1] * d;
		m[3][0] = col[1][0] * col[2][2] * col[3][1] * d + col[1][1] * col[2][0] * col[3][2] * d + col[1][2] * col[2][1] * col[3][0] * d - col[1][0] * col[2][1] * col[3][2] * d - col[1][1] * col[2][2] * col[3][0] * d - col[1][2] * col[2][0] * col[3][1] * d;
		m[3][1] = col[0][0] * col[2][1] * col[3][2] * d + col[0][1] * col[2][2] * col[3][0] * d + col[0][2] * col[2][0] * col[3][1] * d - col[0][0] * col[2][2] * col[3][1] * d - col[0][1] * col[2][0] * col[3][2] * d - col[0][2] * col[2][1] * col[3][0] * d;
		m[3][2] = col[0][0] * col[1][2] * col[3][1] * d + col[0][1] * col[1][0] * col[3][2] * d + col[0][2] * col[1][1] * col[3][0] * d - col[0][0] * col[1][1] * col[3][2] * d - col[0][1] * col[1][2] * col[3][0] * d - col[0][2] * col[1][0] * col[3][1] * d;
		m[3][3] = col[0][0] * col[1][1] * col[2][2] * d + col[0][1] * col[1][2] * col[2][0] * d + col[0][2] * col[1][0] * col[2][1] * d - col[0][0] * col[1][2] * col[2][1] * d - col[0][1] * col[1][0] * col[2][2] * d - col[0][2] * col[1][1] * col[2][0] * d;

		return m;
	}
	template<typename T>
	inline Matrix4<T> Matrix4<T>::identity()
	{
		return Matrix4<T>(
			Col(1, 0, 0, 0),
			Col(0, 1, 0, 0),
			Col(0, 0, 1, 0),
			Col(0, 0, 0, 1)
			);
	}
	template<typename T>
	inline Matrix4<T> Matrix4<T>::zero()
	{
		return Matrix4<T>(
			Col(0, 0, 0, 0),
			Col(0, 0, 0, 0),
			Col(0, 0, 0, 0),
			Col(0, 0, 0, 0)
			);
	}
	template<typename T>
	inline float Matrix4<T>::det() const
	{
		return
			col[0][3] * col[1][2] * col[2][1] * col[3][0] - col[0][2] * col[1][3] * col[2][1] * col[3][0] -
			col[0][3] * col[1][1] * col[2][2] * col[3][0] + col[0][1] * col[1][3] * col[2][2] * col[3][0] +
			col[0][2] * col[1][1] * col[2][3] * col[3][0] - col[0][1] * col[1][2] * col[2][3] * col[3][0] -
			col[0][3] * col[1][2] * col[2][0] * col[3][1] + col[0][2] * col[1][3] * col[2][0] * col[3][1] +
			col[0][3] * col[1][0] * col[2][2] * col[3][1] - col[0][0] * col[1][3] * col[2][2] * col[3][1] -
			col[0][2] * col[1][0] * col[2][3] * col[3][1] + col[0][0] * col[1][2] * col[2][3] * col[3][1] +
			col[0][3] * col[1][1] * col[2][0] * col[3][2] - col[0][1] * col[1][3] * col[2][0] * col[3][2] -
			col[0][3] * col[1][0] * col[2][1] * col[3][2] + col[0][0] * col[1][3] * col[2][1] * col[3][2] +
			col[0][1] * col[1][0] * col[2][3] * col[3][2] - col[0][0] * col[1][1] * col[2][3] * col[3][2] -
			col[0][2] * col[1][1] * col[2][0] * col[3][3] + col[0][1] * col[1][2] * col[2][0] * col[3][3] +
			col[0][2] * col[1][0] * col[2][1] * col[3][3] - col[0][0] * col[1][2] * col[2][1] * col[3][3] -
			col[0][1] * col[1][0] * col[2][2] * col[3][3] + col[0][0] * col[1][1] * col[2][2] * col[3][3];

	}
	template<typename T>
	inline Matrix4<T> Matrix4<T>::TRS(const Vector3<T>& translation, const Quaternion<T> & rotation, const Vector3<T>& scale)
	{
		Matrix4<T> t = Matrix4<T>::identity();
		t[3] = Vector4<T>(translation.x, translation.y, translation.z, 1.f);
		Matrix4<T> r = convert::toMat4(rotation);
		Matrix4<T> s = Matrix4<T>::identity();
		for (int i = 0; i < 3; i++)
			s[i][i] = scale[i];
		return t * (r * s);
	}
	template<typename T>
	inline bool Matrix4<T>::operator==(const Matrix4<T>& c) const
	{
		return (col[0] == c.col[0] && col[1] == c.col[1] && col[2] == c.col[2] && col[3] == c.col[3]);
	}
	template<typename T>
	inline bool Matrix4<T>::operator!=(const Matrix4<T>& c) const
	{
		return (col[0] != c.col[0] || col[1] != c.col[1] || col[2] != c.col[2] || col[3] != c.col[3]);
	}

	template<typename T>
	inline Vector4<T>& Matrix4<T>::operator[](const unsigned int iCol)
	{
		return col[iCol];
	}

	template<typename T>
	inline const Vector4<T>& Matrix4<T>::operator[](const unsigned int iCol) const
	{
		return col[iCol];
	}
}
