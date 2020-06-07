#include "Matte.h"

namespace prim {

Matte::Matte(Texture4f * color) :
	Material(color, BSDFType(BSDF_DIFFUSE | BSDF_REFLECTION))
{
}

color4f Matte::sample(const ComputedIntersection & info, vec3f * wo, float * pdf, BSDFType * type) const
{
	LambertianReflection matte;
	*wo = matte.scatter(info.direction, info.normal);
	*pdf = matte.pdf(*wo, info.normal);
	*type = m_type;
	return matte.evaluate(m_texture->evaluate(info.texcoord), *wo, info.normal);
}

}
