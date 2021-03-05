#pragma once
#include "../../Core/Texture.h"

namespace prim {

template <typename T>
struct Sampler {
	virtual T evaluate(const uv2f& uv = uv2f(0.f)) = 0;
};

template <typename T>
struct LinearSampler : public Sampler<T> {

};

template <typename T>
struct BilinearSampler : public Sampler<T> {

};

template <typename T>
class ImageTexture : public Texture<T> {
public:
	ImageTexture(const T *data, uint32_t width, uint32_t height);

	T evaluate(const uv2f &uv = uv2f(0.f)) const override;

	T at(uint32_t x, uint32_t y) const;

private:
	std::vector<T> m_data;
	uint32_t m_width, m_height;
};

using ImageTexture4f = ImageTexture<color4f>;
using ImageTexture32 = ImageTexture<color32>;
using ImageTextureFloat = ImageTexture<float>;

}

