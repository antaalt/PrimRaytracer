#include "ConstantTexture.h"


namespace raycore {

	ConstantTexture::ConstantTexture(colorHDR albedo) : albedo(albedo)
	{
	}

	colorHDR ConstantTexture::texture2D(float u, float v) const
	{
		return albedo;
	}

	Texture * ConstantTexture::clone() const
	{
		return new ConstantTexture(*this);
	}
}