#pragma once
#include "scientific.h"
namespace geometry {
	struct color32;

	struct colorHDR {
		union {
			math::real_t data[4];
			struct {
				math::real_t r, g, b, a;
			};
		};
		colorHDR();
		explicit colorHDR(math::real_t value);
		explicit colorHDR(math::real_t r, math::real_t g, math::real_t b, math::real_t a);
		explicit colorHDR(const color32 &color);
		math::real_t &operator[](size_t index);
	};

	bool operator==(const colorHDR &lhs, const colorHDR &rhs);
	bool operator!=(const colorHDR &lhs, const colorHDR &rhs);
	colorHDR operator*(const colorHDR &lhs, const colorHDR &rhs);
	colorHDR operator*(const colorHDR &lhs, math::real_t rhs);
	colorHDR operator*(math::real_t rhs, const colorHDR &lhs);
	colorHDR operator/(const colorHDR &lhs, math::real_t rhs);
	colorHDR operator+(const colorHDR &lhs, const colorHDR &rhs);
	colorHDR operator-(const colorHDR &lhs, const colorHDR &rhs);
	colorHDR operator-(const colorHDR &vec);
	colorHDR &operator+=(colorHDR &lhs, const colorHDR &rhs);
	colorHDR &operator-=(colorHDR &lhs, const colorHDR &rhs);
	colorHDR saturate(const colorHDR &vec);
	bool hasNan(const colorHDR & color);

}