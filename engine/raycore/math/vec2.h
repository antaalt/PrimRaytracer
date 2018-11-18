#pragma once
#include "scientific.h"
namespace geometry {
	struct vec3;
	struct vec2 {
		union {
			math::real_t data[2];
			struct {
				math::real_t x, y;
			};
		};
		vec2();
		explicit vec2(math::real_t value);
		explicit vec2(math::real_t x, math::real_t y);
		explicit vec2(const vec3 &vec);
		math::real_t &operator[](size_t index);
		const math::real_t &operator[](size_t index) const;
	};
	math::real_t dot(const vec2 &lhs, const vec2 &rhs);
	math::real_t length(const vec2 &vec);
}