#pragma once
#include "scientific.h"
namespace geometry {

	struct quat {
		union {
			math::real_t data[4];
			struct {
				math::real_t x, y, z, w;
			};
		};
		quat();
		quat(math::real_t x, math::real_t y, math::real_t z, math::real_t w);
		math::real_t &operator[](size_t index);
		static quat identity();
	};
}