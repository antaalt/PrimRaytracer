#pragma once
#include <vector>
#include "Types.h"
#include "Mathematic.h"

#define BILINEAR_FILTER_TEXTURE
#define TEXTURE_REPEAT

namespace raycore {

	inline colorHDR ldr2hdr(const color32 &c)
	{
		return colorHDR(
			powf(c.x / 255.f, 2.2f),
			powf(c.y / 255.f, 2.2f),
			powf(c.z / 255.f, 2.2f),
			powf(c.w / 255.f, 2.2f)
		);
	}

	inline color32 hdr2ldr(const colorHDR &c)
	{
		return color32(
			static_cast<unsigned char>(clamp(powf(c.x, 1 / 2.2f) * 255.f, 0.f, 255.f)),
			static_cast<unsigned char>(clamp(powf(c.y, 1 / 2.2f) * 255.f, 0.f, 255.f)),
			static_cast<unsigned char>(clamp(powf(c.z, 1 / 2.2f) * 255.f, 0.f, 255.f)),
			static_cast<unsigned char>(clamp(c.w * 255.f, 0.f, 255.f))
		);
	}

	class Texture
	{
	public:
		virtual colorHDR texture2D(float u = 0.f, float v = 0.f) const = 0;
		virtual Texture* clone() const = 0;
	};
}