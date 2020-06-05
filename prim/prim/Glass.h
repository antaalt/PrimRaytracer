#pragma once
#include "Material.h"

namespace prim {

struct Glass : public Material {
	Glass(Texture4f *texture, float eta);

	color4f sample(const ComputedIntersection &info, vec3f *wo, float *pdf, BSDFType *type) const override;
private:
	float m_ior;
};

}
