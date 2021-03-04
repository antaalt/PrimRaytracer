#pragma once
#include "Material.h"

namespace prim {

class Metal : public Material {
public:
	Metal(Texture4f *color, TextureFloat *roughness);

	color4f sample(const Intersection &intersection, const vec3f &wi, geometry::vec3f *wo, float *pdf, BSDFType *type) const override;
private:
	TextureFloat *m_roughness;
};

}
