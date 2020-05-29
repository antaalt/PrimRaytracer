#pragma once
#include "Material.h"

namespace raycore {

class Glass : public Material {
public:
	Glass(Texture<float> *texture, float eta);

	virtual color4f sample(const Ray &in, const ComputedIntersection &info, vec3f &wo, float &pdf, BSDFType &type) const;
private:
	float ior;
};

}
