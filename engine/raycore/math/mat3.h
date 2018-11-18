#pragma once
#include "scientific.h"
namespace geometry {
	struct norm3;
	struct point3;
	struct vec3;
	struct quat;
	struct mat4;

	struct mat3 {
		struct col {
			union {
				math::real_t data[3];
				struct {
					math::real_t x, y, z;
				};
			};
			col();
			col(math::real_t value);
			col(math::real_t x, math::real_t y, math::real_t z);
			math::real_t &operator[](size_t index);
			const math::real_t &operator[](size_t index) const;
			operator point3();
			operator norm3();
		} cols[3];
		mat3();
		mat3(math::real_t value);
		mat3(const mat4& mat);
		mat3(col x, col y, col z);
		mat3::col &operator[](size_t index);
		const mat3::col &operator[](size_t index) const;
		static mat3 identity();
	};

	inline mat3 operator*(const mat3& lhs, const mat3 &rhs);
	inline norm3 operator*(const mat3& lhs, const norm3 &rhs);
}