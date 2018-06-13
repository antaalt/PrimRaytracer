#if defined(TYPE_H) && !defined(QUATERNION_H)
#define QUATERNION_H
#include "vector.h"
// TODO make it undependant of vector
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
#endif