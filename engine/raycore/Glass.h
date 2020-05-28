#pragma once
#include "Material.h"

namespace raycore {
namespace prim {

class Glass : public Material {
public:
	Glass(Texture<float> *texture, float eta);

	virtual color4f sample(const tracer::Ray &in, const prim::HitInfo &info, vec3f &wo, float &pdf, BSDFType &type) const;
private:
	float ior;
};

}
}
