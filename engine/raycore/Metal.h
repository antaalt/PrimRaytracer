#pragma once
#include "Material.h"

namespace raycore {

class Metal : public Material {
public:
	Metal(Texture<float> *color, Texture<float> *roughness);

	virtual geometry::color4f sample(const Ray &in, const ComputedIntersection &info, geometry::vec3f &wo, float &pdf, BSDFType &type) const;
private:
	Texture<float> *roughness;
};

}
