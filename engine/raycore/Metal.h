#pragma once
#include "Material.h"

namespace raycore {
namespace prim {

class Metal : public Material {
public:
	Metal(Texture<float> *color, Texture<float> *roughness);

	virtual geometry::color4f sample(const tracer::Ray &in, const prim::HitInfo &info, geometry::vec3f &wo, float &pdf, BSDFType &type) const;
private:
	Texture<float> *roughness;
};
}
}
