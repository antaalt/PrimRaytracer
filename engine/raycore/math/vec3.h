#pragma once
#include "scientific.h"
namespace geometry {
	struct norm3;
	struct point3;
	struct vec2;


	struct vec3 {
		union {
			math::real_t data[3];
			struct {
				math::real_t x, y, z;
			};
		};
		vec3();
		explicit vec3(math::real_t value);
		explicit vec3(math::real_t x, math::real_t y, math::real_t z);
		explicit vec3(const norm3 &normal);
		explicit vec3(const point3 &point);
		explicit vec3(const vec2 &vec, math::real_t z);
		math::real_t &operator[](size_t index);
		const math::real_t &operator[](size_t index) const;
	};

	bool operator==(const vec3 & lhs, const vec3 & rhs);
	bool operator!=(const vec3 &lhs, const vec3 &rhs);
	vec3 operator*(const vec3 &lhs, math::real_t rhs);
	vec3 operator*(math::real_t lhs, const vec3 &rhs);
	vec3 operator/(const vec3 &lhs, math::real_t rhs);
	vec3 operator+(const vec3 &lhs, const vec3 &rhs);
	vec3 operator-(const vec3 &lhs, const vec3 &rhs);
	vec3 operator-(const vec3 &vec);
	float dot(const vec3 & lhs, const vec3 & rhs);
	vec3 cross(const vec3 & lhs, const vec3 & rhs);
	math::real_t length(const vec3 &vec);
	vec3 normalize(const vec3 &vec);
}