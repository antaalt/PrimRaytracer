#include "Glass.h"
#include "Mathematic.h"
namespace raycore {
	namespace prim {
		Glass::Glass(Texture * color, float ior) : 
			Material(color, BSDFType(BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_SPECULAR)),
			ior(ior)
		{
		}
		colorHDR Glass::sample(const tracer::Ray & in, const prim::HitInfo & info, vec3 & wo, float & pdf, BSDFType &type) const
		{
			Specular specular(texture[COLOR_TEXTURE]->texture2D(info.texcoord.x, info.texcoord.y), info, 1.f, ior);
			type = this->type;
			return specular.sample(wo, pdf);
			/*Schlick schlick(1.f, ior);
			SpecularReflection brdf(texture[COLOR_TEXTURE]->texture2D(info.texcoord.x, info.texcoord.y), info, &schlick);
			SpecularTransmission btdf(texture[COLOR_TEXTURE]->texture2D(info.texcoord.x, info.texcoord.y), info, &schlick, ior);
			vec3 refracted = btdf.sample();
			vec3 reflected = brdf.sample();
			if (refracted.isZero()) // TIR
			{
				wo = reflected;
				pdf = brdf.PDF(wo);
				type = BSDF_SPECULAR;
				return brdf.BRDF(wo);
			}
			float R = schlick.evaluate(info.normal, in.direction);
			float z = rand::rnd();
			if (z <= R)
			{ // Reflect
				wo = reflected;
				pdf = brdf.PDF(wo);
				type = BSDF_SPECULAR;
				return brdf.BRDF(wo);
			}
			else
			{ // Refract
				wo = refracted;
				pdf = btdf.PDF(wo);
				type = BSDF_SPECULAR;
				return btdf.BRDF(wo);
			}*/
		}

	}
}