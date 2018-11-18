#pragma once
#include "scientific.h"
namespace geometry {
	struct point3;
	struct vec3;

	struct norm3 {
		union {
			math::real_t data[3];
			struct {
				math::real_t x, y, z;
			};
		};
		norm3();
		explicit norm3(math::real_t value);
		explicit norm3(math::real_t x, math::real_t y, math::real_t z);
		explicit norm3(const vec3 &vec);
		explicit norm3(const point3 &point);
		math::real_t &operator[](size_t index);
	};

	bool operator==(const norm3 &lhs, const norm3 &rhs);
	bool operator!=(const norm3 &lhs, const norm3 &rhs);
	norm3 operator*(const norm3 &lhs, math::real_t rhs);
	norm3 operator*(math::real_t lhs, const norm3 &rhs);
	norm3 operator/(const norm3 &lhs, math::real_t rhs);
	norm3 operator+(const norm3 &lhs, const norm3 &rhs);
	norm3 operator-(const norm3 &lhs, const norm3 &rhs);
	norm3 operator-(const norm3 &vec);
	math::real_t dot(const norm3 &lhs, const norm3 &rhs);
	norm3 normalize(const norm3 &vec);
	norm3 faceForward(const norm3 &n, const vec3 &v);
}