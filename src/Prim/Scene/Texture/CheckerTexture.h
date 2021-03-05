#pragma once
#include "../../Core/Texture.h"

namespace prim {

template <typename T>
class CheckerTexture : public Texture<T> {
public:
	CheckerTexture(T color1, T color2, uint32_t x = 1, uint32_t y = 1);

	T evaluate(const uv2f &uv = uv2f(0.f)) const override;
private:
	T m_color1;
	T m_color2;
	uint32_t m_uscale;
	uint32_t m_vscale;
};

using CheckerTextureFloat = CheckerTexture<float>;
using CheckerTexture4f = CheckerTexture<color4f>;

}