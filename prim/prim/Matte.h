#pragma once
#include "Material.h"
	
namespace prim {

struct Matte : public Material {
	Matte(Texture<float> *color);
	color4f sample(const ComputedIntersection &info, vec3f *wo, float *pdf, BSDFType *type) const override;
};

}


