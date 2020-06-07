#include "Glass.h"

namespace prim {

Glass::Glass(Texture4f * color, float ior) : 
	Material(color, BSDFType(BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_SPECULAR)),
	m_ior(ior)
{
}

color4f Glass::sample(const ComputedIntersection & info, vec3f *wo, float *pdf, BSDFType *type) const
{
	Specular specular(m_ior);
	*wo = specular.scatter(info.direction, info.normal);
	*pdf = specular.pdf(*wo, info.normal);
	*type = m_type;
	return specular.evaluate(m_texture->evaluate(info.texcoord), *wo, info.normal);
}

}