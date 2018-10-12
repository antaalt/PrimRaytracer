#pragma once
#include "Texture.h"

namespace raycore {


	class Checker : public Texture {
	public:
		Checker(colorHDR color1, colorHDR color2, unsigned int x = 1, unsigned int y = 1);
		virtual colorHDR texture2D(float u = 0.f, float v = 0.f) const;
		virtual Texture* clone() const;
	private:
		colorHDR color1, color2;
		unsigned int x, y;
	};

}