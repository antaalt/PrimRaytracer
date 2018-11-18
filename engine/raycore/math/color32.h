#pragma once
#include "scientific.h"
namespace geometry {
	struct colorHDR;

	struct color32 {
		union {
			math::byte_t data[4];
			struct {
				math::byte_t r, g, b, a;
			};
		};
		color32();
		explicit color32(math::byte_t value);
		explicit color32(math::byte_t r, math::byte_t g, math::byte_t b, math::byte_t a);
		explicit color32(const colorHDR &color);
		math::byte_t &operator[](size_t index);
	};
}