#pragma once
#include "scientific.h"
namespace geometry {
	struct norm3;
	struct vec3;

	struct point3 {
		union {
			math::real_t data[3];
			struct {
				math::real_t x, y, z;
			};
		};
		point3();
		explicit point3(math::real_t value);
		explicit point3(math::real_t x, math::real_t y, math::real_t z);
		explicit point3(const norm3 &normal);
		explicit point3(const vec3 &vec);
		math::real_t &operator[](size_t index);
	};

	
	bool operator==(const point3 &lhs, const point3 &rhs);
	bool operator!=(const point3 &lhs, const point3 &rhs);
	point3 operator*(const point3 &lhs, math::real_t rhs);
	point3 operator*(math::real_t lhs, const point3 &rhs);
	point3 operator/(const point3 &lhs, math::real_t rhs);
	point3 operator+(const point3 &lhs, const point3 &rhs);
	point3 operator-(const point3 &lhs, const point3 &rhs);
	point3 operator-(const point3 &vec);
	point3 &operator+=(point3 &lhs, const point3 &rhs);
	point3 &operator-=(point3 &lhs, const point3 &rhs);
	point3 &operator/=(point3 &lhs, float rhs);
	math::real_t distance(const point3 &lhs, const point3 &rhs);
}