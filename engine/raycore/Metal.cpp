#include "Metal.h"

namespace raycore {
namespace prim {
Metal::Metal(Texture<float> * color, Texture<float> *roughness) : 
	Material(color, BSDFType(BSDF_SPECULAR | BSDF_REFLECTION | BSDF_GLOSSY)),
	roughness(roughness)
{
}
color4f Metal::sample(const tracer::Ray & in, const prim::HitInfo & info, geometry::vec3f & wo, float & pdf, BSDFType &type) const
{
	// TODO 1D textures
	MicrofacetReflection brdf(colorTexture->evaluate(info.texcoord), info, roughness->evaluate(info.texcoord).r);
	type = this->type;
	return brdf.sample(wo, pdf);
}
}
}