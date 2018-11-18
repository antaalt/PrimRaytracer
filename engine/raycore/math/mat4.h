#pragma once
#include "scientific.h"
namespace geometry {
	struct norm3;
	struct point3;
	struct vec3;
	struct quat;

	struct mat4 {
		struct col {
			union {
				math::real_t data[4];
				struct {
					math::real_t x, y, z, w;
				};
			};
			col();
			col(math::real_t value);
			col(math::real_t x, math::real_t y, math::real_t z, math::real_t w);
			col(norm3 vec, math::real_t w);
			col(vec3 vec, math::real_t w);
			col(point3 vec, math::real_t w);
			math::real_t &operator[](size_t index);
			const math::real_t &operator[](size_t index) const;
			operator point3();
			operator norm3();
		} cols[4];
		mat4();
		mat4(math::real_t value);
		mat4(col x, col y, col z, col w);
		mat4(const quat &quat);
		mat4::col &operator[](size_t index);
		const mat4::col &operator[](size_t index) const;
		static mat4 identity();
		static mat4 translate(const vec3 &translation);
		static mat4 rotate(const vec3 &axis, math::Radian angle);
		static mat4 scale(const vec3 &scale);
		float det() const;
	};

	mat4 operator*(const mat4& lhs, const mat4 &rhs);
	point3 operator*(const mat4& lhs, const point3 &rhs);
	vec3 operator*(const mat4& lhs, const vec3 &rhs);
	mat4 TRS(const vec3 & t, const quat & r, const vec3 & s);
	mat4 inverse(const mat4 &mat);
}