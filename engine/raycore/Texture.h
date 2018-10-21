#pragma once
#include <vector>
#include "Config.h"

#define BILINEAR_FILTER_TEXTURE
#define TEXTURE_REPEAT

namespace raycore {

	template <typename T>
	class Texture
	{
	public:
		virtual T evaluate(const uv2 &uv = uv2(0.f)) const = 0;
		virtual Texture* clone() const = 0;
	};
}