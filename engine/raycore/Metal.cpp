#include "Metal.h"

namespace raycore {
	namespace prim {
		Metal::Metal(Texture<colorHDR> * color, Texture<float> *roughness) : 
			Material(color, BSDFType(BSDF_SPECULAR | BSDF_REFLECTION | BSDF_GLOSSY)),
			roughness(roughness)
		{
		}
		colorHDR Metal::sample(const tracer::Ray & in, const prim::HitInfo & info, vec3 & wo, float & pdf, BSDFType &type) const
		{
			MicrofacetReflection brdf(colorTexture->evaluate(info.texcoord), info, roughness->evaluate(info.texcoord));
			type = this->type;
			return brdf.sample(wo, pdf);
		}
	}
}