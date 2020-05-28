#include "Glass.h"

namespace raycore {
namespace prim {

Glass::Glass(Texture<float> * color, float ior) : 
	Material(color, BSDFType(BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_SPECULAR)),
	ior(ior)
{
}
color4f Glass::sample(const tracer::Ray & in, const prim::HitInfo & info, vec3f & wo, float & pdf, BSDFType &type) const
{
	Specular specular(colorTexture->evaluate(info.texcoord), info, 1.f, ior);
	type = this->type;
	return specular.sample(wo, pdf);
}

}
}