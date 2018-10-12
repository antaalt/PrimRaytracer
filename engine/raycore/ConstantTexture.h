#pragma once
#include "Texture.h"

namespace raycore {
	class ConstantTexture : public Texture {
	public:
		ConstantTexture(colorHDR albedo);
		virtual colorHDR texture2D(float u = 0.f, float v = 0.f) const;
		virtual Texture* clone() const;
	private:
		colorHDR albedo;
	};
}

