#pragma once
#include "scientific.h"
namespace geometry {
	struct uv2 {
		union {
			math::real_t data[2];
			struct {
				math::real_t u, v;
			};
		};
		uv2();
		explicit uv2(math::real_t value);
		explicit uv2(math::real_t u, math::real_t v);
		math::real_t &operator[](size_t index);
	};
	bool operator==(const uv2 &lhs, const uv2 &rhs);
	bool operator!=(const uv2 &lhs, const uv2 &rhs);
	uv2 operator*(const uv2 &lhs, math::real_t rhs);
	uv2 operator*(math::real_t lhs, const uv2 &rhs);
	uv2 operator/(const uv2 &lhs, math::real_t rhs);
	uv2 operator+(const uv2 &lhs, const uv2 &rhs);
	uv2 operator-(const uv2 &lhs, const uv2 &rhs);
	uv2 operator-(const uv2 &vec);
}