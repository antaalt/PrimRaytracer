#include "Matte.h"

namespace prim {

Matte::Matte(Texture4f * color) :
	Material(color, BSDFType(BSDF_DIFFUSE | BSDF_REFLECTION))
{
}

color4f Matte::sample(const Intersection &intersection, const vec3f &wi, vec3f * wo, float * pdf, BSDFType * type) const
{
	LambertianReflection matte;
	*wo = matte.scatter(wi, intersection.normal);
	*pdf = matte.pdf(*wo, intersection.normal);
	*type = m_type;
	return matte.evaluate(m_texture->evaluate(intersection.texcoord), *wo, intersection.normal);
}

}
