#pragma once
#include "Texture.h"

namespace raycore {

template <typename T>
class Checker : public Texture<T> {
public:
	Checker(geometry::color4<T> color1, geometry::color4<T> color2, unsigned int x = 1, unsigned int y = 1);

	geometry::color4<T> evaluate(const geometry::uv2f &uv = geometry::uv2f(0.f)) const override;
	Texture* clone() const override;
private:
	geometry::color4<T> m_color1, m_color2;
	unsigned int m_uscale, m_vscale;
};

}