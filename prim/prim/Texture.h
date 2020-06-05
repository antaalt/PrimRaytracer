#pragma once
#include <vector>
#include "Config.h"

#include "math/geometry.h"

namespace prim {

template <typename T>
class Texture
{
public:
	Texture() {}
	Texture(const Texture &) = delete;
	Texture &operator=(const Texture &) = delete;
	virtual ~Texture() {}

	virtual T evaluate(const uv2f &uv = uv2f(0.f)) const = 0;

	template <typename U>
	U evaluate(const uv2f &uv = uv2f(0.f)) const {
		return 0.f;
	}
};

using Texture32 = Texture<color4<uint8_t>>;
using Texture4f = Texture<color4<float>>;
using TextureFloat = Texture<float>;
using TextureByte = Texture<uint8_t>;

}