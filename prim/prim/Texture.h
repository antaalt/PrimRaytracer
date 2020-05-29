#pragma once
#include <vector>
#include "Config.h"

#include "math/geometry.h"

#define BILINEAR_FILTER_TEXTURE
#define TEXTURE_REPEAT

namespace prim {

template <typename T>
class Texture
{
public:
	virtual geometry::color4<T> evaluate(const geometry::uv2f &uv = geometry::uv2f(0.f)) const = 0;
	virtual Texture* clone() const = 0;
};

}