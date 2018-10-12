#include "Checker.h"


namespace raycore {

	Checker::Checker(colorHDR color1, colorHDR color2, unsigned int x, unsigned int y) : color1(color1), color2(color2), x(x), y(y)
	{
	}

	colorHDR Checker::texture2D(float u, float v) const
	{
		unsigned int u2 = static_cast<unsigned int>(u * 2.f * x) % 2;
		unsigned int v2 = static_cast<unsigned int>(v * 2.f * y) % 2;
		//if ((u > 0.5f && v < 0.5f) || (u < 0.5f && v > 0.5f))
		if ((u2 == 0 && v2 == 1) || (u2 == 1 && v2 == 0))
			return color2;
		return color1;
	}

	Texture * Checker::clone() const
	{
		return new Checker(*this);
	}

}