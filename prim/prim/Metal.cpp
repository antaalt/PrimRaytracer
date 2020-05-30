#include "Metal.h"

namespace prim {

Metal::Metal(Texture<float> * color, Texture<float> *roughness) : 
	Material(color, BSDFType(BSDF_SPECULAR | BSDF_REFLECTION | BSDF_GLOSSY)),
	m_roughness(roughness)
{
}
color4f Metal::sample(const ComputedIntersection & info, geometry::vec3f * wo, float * pdf, BSDFType * type) const
{
	MicrofacetReflection microfacet(m_roughness->evaluate(info.texcoord).r);
	*wo = microfacet.scatter(info.direction, info.normal);
	*pdf = microfacet.pdf(*wo, info.normal);
	*type = m_type;
	return microfacet.evaluate(info.color * m_texture->evaluate(info.texcoord), *wo, info.normal);
}

}