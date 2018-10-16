#include "Matte.h"

namespace raycore {
	namespace prim{
		Matte::Matte(Texture * color) : 
			Material(color, BSDFType(BSDF_DIFFUSE | BSDF_REFLECTION))
		{
		}
		colorHDR Matte::sample(const tracer::Ray & in, const prim::HitInfo & info, vec3 & wo, float & pdf, BSDFType &type) const
		{
			LambertianReflection brdf(texture[COLOR_TEXTURE]->texture2D(info.texcoord.x, info.texcoord.y), info);
			type = this->type;
			return brdf.sample(wo, pdf);
		}
	}
}
