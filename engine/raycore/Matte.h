#pragma once
#include "Material.h"
	
namespace raycore {

class Matte : public Material {
public:
	Matte(Texture<float> *color);
	virtual color4f sample(const Ray &in, const ComputedIntersection &info, vec3f &wo, float &pdf, BSDFType &type) const;
};

}


