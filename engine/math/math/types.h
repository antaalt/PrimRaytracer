#if !defined(TYPE_H)
#define TYPE_H

#if defined(USE_QUATERNION) && !defined(USE_VECTOR)
#define USE_VECTOR
#endif
#if defined(USE_MATRIX) && !defined(USE_VECTOR)
#define USE_VECTOR
#endif

#if defined(DEBUG)
#define ASSERT(condition, message)
#else
#define ASSERT(condition, message)
#endif

#if defined(USE_VECTOR)
#include "vector.h"
typedef Type::Vector<float, 2>			Vector2f;
typedef Type::Vector<float, 3>			Vector3f;
typedef Type::Vector<float, 4>			Vector4f;
typedef Type::Vector<unsigned char, 4>	Vector4u;
typedef Type::Vector<unsigned char, 4>	Color32;
typedef Type::Vector<float, 4>			Color4f;
#endif
#if defined(USE_MATRIX)
#include "matrix.h"
typedef Type::Matrix<float, 2>			Matrix2f;
typedef Type::Matrix<float, 3>			Matrix3f;
typedef Type::Matrix<float, 4>			Matrix4f;
#endif
#if defined(USE_QUATERNION)
#include "quaternion.h"
typedef Type::Quaternion<float>			Quaternionf;
typedef Type::Quaternion<double>		Quaterniond;
#endif

namespace Type {
#if defined(USE_MATRIX) && defined(USE_VECTOR) && defined(USE_QUATERNION)
	template <typename T>
	inline Matrix<T, 4> TRS(Vector<T, 4> &translation, Quaternion<T> &rotation, Vector<T, 4> &scale)
	{

	}
#endif

#if defined(USE_MATRIX) && defined(USE_VECTOR)
	template <typename T>
	void translate(const Matrix<T, 4> &mat, const Vector<T, 3> &vec)
	{

	}
	template <typename T>
	void rotate(const Matrix<T, 4> &mat, const T angle, const Vector<T, 4> &dir)
	{

	}
	template <typename T>
	void scale(const Matrix<T, 4> &mat, const Vector<T, 4> &vec)
	{

	}
#endif
#if defined(USE_MATRIX) && defined(USE_QUATERNION)
	template <typename T>
	void rotate(const Matrix<T, 4> &mat, const Quaternion<T> &quat)
	{

	}

	template <typename T>
	Matrix<T, 4> toMat4(Quaternion<T> &quat)
	{

	}
	template <typename T>
	Quaternion<T> toQuat(Matrix<T, 4> &mat)
	{

	}
#endif


#if defined(USE_VECTOR)
	template <typename T>
	Vector<T, 2> toVec2(Vector<T, 3> &vec) { return Vector<T, 2>(vec.x, vec.y); }
	template <typename T>
	Vector<T, 2> toVec2(Vector<T, 4> &vec) { return Vector<T, 2>(vec.x, vec.y); }

	template <typename T>
	Vector<T, 3> toVec3(Vector<T, 2> &vec, T z) { return Vector<T, 3>(vec.x, vec.y, z); }
	template <typename T>
	Vector<T, 3> toVec3(Vector<T, 4> &vec) { return Vector<T, 3>(vec.x, vec.y, vec.z); }

	template <typename T>
	Vector<T, 4> toVec4(Vector<T, 2> &vec, T z, T w) { return Vector<T, 4>(vec.x, vec.y, z, w); }
	template <typename T>
	Vector<T, 4> toVec4(Vector<T, 3> &vec, T w) { return Vector<T, 4>(vec.x, vec.y, vec.z, w); }

	template <typename T, int N>
	inline float dot(const Vector<T, N> &lhs, const Vector<T, N> &rhs) { return lhs.x*rhs.x + lhs.y*rhs.y; }
	/*template <typename T>
	inline float dot(const Vector<T, 3> &lhs, const Vector<T, 3> &rhs) { return x*x + y*y + z*z; }
	template <typename T>
	inline float dot(const Vector<T, 4> &lhs, const Vector<T, 4> &rhs) { return x*x + y*y + z*z + w*w; }*/

	template <typename T, int N>
	float cross(const Vector<T, N> &vec) { return 0.f; }
#endif

}
#endif