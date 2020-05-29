#pragma once
#include "Texture.h"

namespace prim {

template <typename T, typename U>
class TTextureMap : public Texture<T> {
public:
	TTextureMap(const std::vector<U> &data, unsigned int width, unsigned int height);
	TTextureMap(const U *data, unsigned int width, unsigned int height);

	geometry::color4<T> evaluate(const geometry::uv2f &uv = geometry::uv2f(0.f)) const override;
	geometry::color4<T> at(unsigned int x, unsigned int y) const;

	unsigned int stride() const;

	virtual Texture* clone() const;
private:
	std::vector<U> m_data;
	unsigned int m_width, m_height;
	unsigned int m_components;
};

using TextureMapFloat = TTextureMap<float, float>;
using TextureMapFloat32 = TTextureMap<float, unsigned char>;

}

