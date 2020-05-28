#pragma once
#include "Texture.h"

namespace raycore {

template <typename T>
class ConstantTexture : public Texture<T> {
public:
	ConstantTexture(geometry::color4<T> albedo);
	virtual geometry::color4<T> evaluate(const geometry::uv2f &uv = geometry::uv2f(0.f)) const;
	virtual Texture* clone() const;
private:
	geometry::color4<T> albedo;
};

}

