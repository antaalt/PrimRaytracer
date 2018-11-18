#include "point3.h"

namespace geometry {
	inline quat::quat()
	{
	}
	inline quat::quat(math::real_t x, math::real_t y, math::real_t z, math::real_t w) : x(x), y(y), z(z), w(w)
	{
	}
	inline math::real_t & quat::operator[](size_t index)
	{
		return data[index];
	}
	inline quat quat::identity()
	{
		return quat(0.f, 0.f, 0.f, 1.f);
	}
}