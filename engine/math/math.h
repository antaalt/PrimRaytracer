//-------------
// vector.h
//-------------
#include <ostream>
namespace Type {

	namespace {
		constexpr bool isDisplayable(int N, int limit)
		{
			return N > limit;
		}
	}

	template <typename T, int N>
	class Vector {
		static_assert(N > 1 && N < 5, "Vector support only 2, 3, 4 components");
	public:
		Vector() {}
		Vector(const T a1, const T a2);
		Vector(const T a1, const T a2, const T a3);
		Vector(const T a1, const T a2, const T a3, const T a4);
		Vector(const Vector<T, N> &vec);
		~Vector() {}

		T &operator[](const int component);
		const T &operator[](const int component) const;

		inline bool operator<(const Vector<T, N> &v) const;
		inline bool operator>(const Vector<T, N> &v) const;

		inline bool operator==(const Vector<T, N> &v) const;
		inline bool operator!=(const Vector<T, N> &v) const;

		template <typename T, int N, typename C>
		friend Vector<T, N> operator*(const Vector<T, N> &vec, const C scalar);

		template <typename T, int N, typename C>
		friend Vector<T, N> operator/(const Vector<T, N> &vec, const C scalar);

		inline Vector<T, N> operator+(const Vector<T, N> &i) const;
		inline Vector<T, N> operator+(const T &val) const;
		inline Vector<T, N> operator-(const Vector<T, N> &i) const;
		inline Vector<T, N> operator-(const T &val) const;

		inline Vector<T, N> operator+(const Vector<T, N> &i) const;
		inline Vector<T, N> operator+(const T &val) const;
		inline Vector<T, N> operator-(const Vector<T, N> &i) const;
		inline Vector<T, N> operator-(const T &val) const;

		template <typename T, int N>
		friend std::ostream& operator <<(std::ostream& os, const Vector<T, N>& vec);

	public:
		float dot(const Vector<T, N> &vec) const;
		float length() const;
		Vector<T, N> reverse() const;

	public:
		union {
			struct {
				T x;
				T y;
				typename std::enable_if<isDisplayable(N, 2), T>::type z;
				typename std::enable_if<isDisplayable(N, 3), T>::type w;
			};
			T data[N];
		};
	};
}

//-------------
// quaternion.h
//-------------
//#include "vector.h"
namespace Type {

	template <typename T>
	class Quaternion : public Vector<T, 4>
	{
		static_assert(std::is_floating_point<T>::value, "Template must be floating point");
	public:
		static Quaternion<T> fromEuler(const T x, const T y, const T z);
		Vector<T, 3> euler();
	};
}

//-------------
// matrix.h
//-------------
#include <ostream>
namespace Type {

	template <typename T, int N>
	class Matrix {
		static_assert(N > 1 && N < 5, "Matrix support only 2, 3, 4 dimensions");
	public:
		Vector<T, N> &operator[](const int col);
		const Vector<T, N> &operator[](const int col) const;

		template <typename T, int N>
		friend std::ostream& operator <<(std::ostream& os, const Matrix<T, N>& mat);

	public:
		inline Matrix<T, N> transpose() const;
		inline Matrix<T, N> inverse() const;

		static Matrix<T, N> identity();

	private:
		union {
			Vector<T, N> m_cols[N];
			T m_data[N * N];
		};
	};
}

//-------------
// type.h
//-------------
/*#include "vector.h"
#include "matrix.h"
#include "quaternion.h"*/
/*typedef Type::Vector<float, 2>			Vector2f;
typedef Type::Vector<float, 3>			Vector3f;
typedef Type::Vector<float, 4>			Vector4f;
typedef Type::Vector<unsigned char, 4>	Vector4u;
typedef Type::Matrix<float, 4>			Matrix4f;
typedef Type::Matrix<float, 3>			Matrix3f;
typedef Type::Quaternion<float>			Quaternionf;
typedef Type::Quaternion<double>		Quaterniond;*/

namespace Type {

	template <typename T>
	inline Matrix<T, 4> TRS(Vector<T, 4> &translation, Quaternion<T> &rotation, Vector<T, 4> &scale);

	template <typename T>
	void translate(const Matrix<T, 4> &mat, const Vector<T, 3> &vec);
	template <typename T>
	void rotate(const Matrix<T, 4> &mat, const T angle, const Vector<T, 4> &angle);
	template <typename T>
	void rotate(const Matrix<T, 4> &mat, const Quaternion<T> &quat);
	template <typename T>
	void scale(const Matrix<T, 4> &mat, const Vector<T, 4> &vec);

	template <typename T>
	Vector<T, 2> toVec2(Vector<T, 3> &vec);
	template <typename T>
	Vector<T, 2> toVec2(Vector<T, 4> &vec);

	template <typename T>
	Vector<T, 3> toVec3(Vector<T, 2> &vec, T data);
	template <typename T>
	Vector<T, 3> toVec3(Vector<T, 4> &vec);

	template <typename T>
	Vector<T, 4> toVec4(Vector<T, 2> &vec, T data1, T data2);
	template <typename T>
	Vector<T, 4> toVec4(Vector<T, 3> &vec, T data);

	template <typename T>
	Matrix<T, 4> toMat4(Quaternion<T> &quat);
	template <typename T>
	Quaternion<T> toQuat(Matrix<T, 4> &mat);
}