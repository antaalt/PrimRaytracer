#pragma once
#include "Material.h"
	
namespace raycore {
namespace prim {

class Matte : public Material {
public:
	Matte(Texture<float> *color);
	virtual color4f sample(const tracer::Ray &in, const prim::HitInfo &info, vec3f &wo, float &pdf, BSDFType &type) const;
};
}
}


