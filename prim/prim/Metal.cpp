#include "Metal.h"

namespace prim {

Metal::Metal(Texture4f * color, TextureFloat *roughness) : 
	Material(color, BSDFType(BSDF_SPECULAR | BSDF_REFLECTION | BSDF_GLOSSY)),
	m_roughness(roughness)
{
}
color4f Metal::sample(const Intersection &intersection, const vec3f &wi, geometry::vec3f * wo, float * pdf, BSDFType * type) const
{
	MicrofacetReflection microfacet(m_roughness->evaluate(intersection.texcoord));
	*wo = microfacet.scatter(wi, intersection.normal);
	*pdf = microfacet.pdf(*wo, intersection.normal);
	*type = m_type;
	return microfacet.evaluate(m_texture->evaluate(intersection.texcoord), *wo, intersection.normal);
}

}