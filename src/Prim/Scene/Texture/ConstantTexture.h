#pragma once
#include "../../Core/Texture.h"

namespace prim {

template <typename T>
class ConstantTexture : public Texture<T> {
public:
	ConstantTexture(T albedo);

	T evaluate(const uv2f &uv = uv2f(0.f)) const override;

private:
	T m_albedo;
};

using ConstantTextureFloat = ConstantTexture<float>;
using ConstantTexture4f = ConstantTexture<color4f>;

}

