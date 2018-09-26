#pragma once

#include "Vector3.h"
#include "Quaternion.h"

namespace Math {

	template <typename T>
	struct Matrix3 {
		using Col = Vector3<T>;
		union {
			T data[3 * 3];
			struct {
				T	xx, yx, zx,
					xy, yy, zy,
					xz, yz, zz;
			};
			Vector3<T> col[3];
		};
		Matrix3();
		Matrix3(const Col &col1, const Col &col2, const Col &col3);
		Matrix3(const T * data);

		inline Matrix3<T> transpose() const;
		//inline Matrix3<T> inverse() const;
		static Matrix3<T> identity();
		static Matrix3<T> zero();

		//inline float det() const;

		template <typename T>
		friend Matrix3<T> operator*(const Matrix3<T> &lhs, const Matrix3<T> &rhs);

		template <typename T>
		friend Vector3<T> operator*(const Matrix3<T> &lhs, const Vector3<T> &rhs);

		inline bool operator==(const Matrix3<T> &c) const;
		inline bool operator!=(const Matrix3<T> &c) const;

		inline Col &operator[](const unsigned int col);
		inline const Col &operator[](const unsigned int col) const;
	};

	template<typename T>
	inline Matrix3<T> operator*(const Matrix3<T>& lhs, const Matrix3<T>& rhs)
	{
		Matrix3<T> out = Matrix3<T>::zero();
		for (int iCol = 0; iCol < 3; iCol++)
			for (int iRow = 0; iRow < 3; iRow++)
				for (int k = 0; k < 3; k++)
					out[iCol][iRow] += rhs[iCol][k] * lhs[k][iRow];
		return out;
	}

	template<typename T>
	inline Vector3<T> operator*(const Matrix3<T> &lhs, const Vector3<T> &rhs)
	{
		return Vector3<T>(
			lhs.col[0].x * rhs.x + lhs.col[1].x * rhs.y + lhs.col[2].x * rhs.z,
			lhs.col[0].y * rhs.x + lhs.col[1].y * rhs.y + lhs.col[2].y * rhs.z,
			lhs.col[0].z * rhs.x + lhs.col[1].z * rhs.y + lhs.col[2].z * rhs.z
		);
	}

	template<typename T>
	inline Matrix3<T>::Matrix3()
	{
	}

	template<typename T>
	inline Matrix3<T>::Matrix3(const Col & col1, const Col & col2, const Col & col3)
	{
		col[0] = col1;
		col[1] = col2;
		col[2] = col3;
	}

	template<typename T>
	inline Matrix3<T>::Matrix3(const T * data)
	{
		for (int iCol = 0; iCol < 3; iCol++)
			for (int iRow = 0; iRow < 3; iRow++)
				col[iCol][iRow] = data[iCol * 3 + iRow];
	}

	template<typename T>
	inline Matrix3<T> Matrix3<T>::transpose() const
	{
		Matrix3<T> out;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				out[j][i] = col[i][j];
		return out;
	}
	/*template<typename T>
	inline Matrix3<T> Matrix3<T>::inverse() const
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

		Matrix3<T> m;
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
	}*/
	template<typename T>
	inline Matrix3<T> Matrix3<T>::identity()
	{
		return Matrix3<T>(
			Col(1, 0, 0),
			Col(0, 1, 0),
			Col(0, 0, 1)
		);
	}
	template<typename T>
	inline Matrix3<T> Matrix3<T>::zero()
	{
		return Matrix3<T>(
			Col(0, 0, 0),
			Col(0, 0, 0),
			Col(0, 0, 0),
		);
	}
	/*template<typename T>
	inline float Matrix3<T>::det() const
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

	}*/
	template<typename T>
	inline bool Matrix3<T>::operator==(const Matrix3<T>& c) const
	{
		return (col[0] == c.col[0] && col[1] == c.col[1] && col[2] == c.col[2]);
	}
	template<typename T>
	inline bool Matrix3<T>::operator!=(const Matrix3<T>& c) const
	{
		return (col[0] != c.col[0] || col[1] != c.col[1] || col[2] != c.col[2]);
	}

	template<typename T>
	inline Vector3<T>& Matrix3<T>::operator[](const unsigned int iCol)
	{
		return col[iCol];
	}

	template<typename T>
	inline const Vector3<T>& Matrix3<T>::operator[](const unsigned int iCol) const
	{
		return col[iCol];
	}
}
