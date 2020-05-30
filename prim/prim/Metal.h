#pragma once
#include "Material.h"

namespace prim {

class Metal : public Material {
public:
	Metal(Texture<float> *color, Texture<float> *roughness);

	color4f sample(const ComputedIntersection &info, geometry::vec3f *wo, float *pdf, BSDFType *type) const override;
private:
	Texture<float> *m_roughness;
};

}
