#include "Glass.h"

namespace raycore {
	namespace prim {
		Glass::Glass(Texture<colorHDR> * color, float ior) : 
			Material(color, BSDFType(BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_SPECULAR)),
			ior(ior)
		{
		}
		colorHDR Glass::sample(const tracer::Ray & in, const prim::HitInfo & info, vec3 & wo, float & pdf, BSDFType &type) const
		{
			Specular specular(colorTexture->evaluate(info.texcoord), info, 1.f, ior);
			type = this->type;
			return specular.sample(wo, pdf);
		}

	}
}