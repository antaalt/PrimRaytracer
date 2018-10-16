#include "Metal.h"

namespace raycore {
	namespace prim {
		Metal::Metal(Texture * color, float roughness) : 
			Material(color, BSDFType(BSDF_SPECULAR | BSDF_REFLECTION | BSDF_GLOSSY)),
			roughness(roughness)
		{
		}
		colorHDR Metal::sample(const tracer::Ray & in, const prim::HitInfo & info, vec3 & wo, float & pdf, BSDFType &type) const
		{
			MicrofacetReflection brdf(texture[COLOR_TEXTURE]->texture2D(info.texcoord.x, info.texcoord.y), info, roughness);
			/*vec3 m = brdf.sample();
			
			wo = reflect(in.direction, m);
			if (vec3::dot(wo, info.normal) < 0.f)
			{
				pdf = 1;
				type = BSDF_REFLECTION;
				return colorHDR(0.f);
			}
			pdf = brdf.PDF(wo);
			type = BSDF_REFLECTION;
			return brdf.BRDF(wo);*/
			type = this->type;
			return brdf.sample(wo, pdf);

			/*float r1 = rand::rnd();
			float r2 = rand::rnd();
			float theta = atanf(roughness * sqrtf(r1) / sqrtf(1 - r1));
			float phi = 2 * M_PIf * r2;
			vec3 m;
			m.x = cosf(phi) * sinf(theta);
			m.z = sinf(phi) * sinf(theta);
			m.y = cosf(theta);
			transform::Onb onb(info.normal);
			vec3 mT = onb(m);
			wo = reflect(in.direction, mT);
			pdf = vec3::dot(wo, info.normal) / M_PIf;
			type = BSDF_GLOSSY;
			return texture[COLOR_TEXTURE]->texture2D(info.texcoord.x, info.texcoord.y) / M_PIf;*/
		}
	}
}