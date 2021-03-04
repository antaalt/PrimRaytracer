#pragma once
#include "../../Core/Material.h"

namespace prim {

struct Glass : public Material {
	Glass(Texture4f *texture, float eta);

	color4f sample(const Intersection &intersection, const vec3f &wi, vec3f *wo, float *pdf, BSDFType *type) const override;
private:
	float m_ior;
};

}
