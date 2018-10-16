#pragma once
#include "Types.h"
#include "Ray.h"
#include "Hitable.h"
#include "Random.h"
#include "Onb.h"
#include "Fresnel.h"

namespace raycore {

	vec3 reflect(const vec3 &wi, const norm3 &normal);
	bool refract(vec3 &out, const vec3 &wi, const norm3 &normal, float eta);
	vec3 sampleUnitSphere(float r1, float r2);
	vec3 sampleMicroFacet(float roughness, float r1, float r2);

	class BSDF
	{
	public:
		BSDF(const prim::HitInfo &info) : normal(info.normal), wi(info.direction) {}
		virtual colorHDR sample(vec3 &wo, float &pdf) const = 0;
	protected:
		virtual float PDF(const vec3 &wo) const = 0;
		virtual colorHDR evaluate(const vec3 &wo) const = 0;
	protected:
		norm3 normal;
		vec3 wi;
	};

	class BRDF : protected BSDF {
	public:
		BRDF(const prim::HitInfo &info) : BSDF(info) {}
	};

	class BTDF : protected BSDF {
	public:
		BTDF(const prim::HitInfo &info) : BSDF(info) {}

	};

	class LambertianReflection : BRDF
	{
	public:
		LambertianReflection(const colorHDR &color, const prim::HitInfo &info) : BRDF(info), Rd(color) {}
		
		virtual colorHDR sample(vec3 &wo, float &pdf) const
		{
			float r1 = rand::rnd();
			float r2 = rand::rnd();
			vec3 randomDirection = sampleUnitSphere(r1, r2);
			transform::Onb onb(normal);
			wo = onb(randomDirection);
			pdf = this->PDF(wo);
			return this->evaluate(wo);
		}
	protected:
		virtual float PDF(const vec3 &wo) const
		{
			return vec3::dot(wo, normal) / M_PIf;
		}
		virtual colorHDR evaluate(const vec3 &wo) const
		{
			return Rd / M_PIf;
		}
	private:
		colorHDR Rd;
	};

	class SpecularReflection : BRDF
	{
	public:
		SpecularReflection(const colorHDR &color, const prim::HitInfo &info) :
			BRDF(info), Rs(color)
		{
		}
		virtual colorHDR sample(vec3 &wo, float &pdf) const
		{
			wo = reflect(wi, normal);
			pdf = this->PDF(wo);
			return this->evaluate(wo);
		}
	protected:
		virtual float PDF(const vec3 &wo) const
		{
			return 1.f;
		}
		virtual colorHDR evaluate(const vec3 &wo) const
		{
			return Rs / vec3::dot(wo, normal);
		}
	private:
		colorHDR Rs;
	};

	class Specular : BSDF
	{
	public:
		Specular(const colorHDR &color, const prim::HitInfo &info, float etaOut, float etaIn) :
			BSDF(info), Ts(color), etaOut(etaOut), etaIn(etaIn)
		{
		}
		virtual colorHDR sample(vec3 &wo, float &pdf) const
		{
			vec3 nn = normal;

			float etar = 1.f / etaIn;
			if (vec3::dot(wi, normal) >= 0.f)
			{
				etar = etaIn;
				nn = -normal;
			}

			vec3 refracted;
			vec3 reflected = reflect(wi, normal);
			if (refract(refracted, wi, nn, etar)) // TIR
			{
				wo = reflected;
				pdf = this->PDF(wo);
				return this->evaluate(wo);
			}
			Schlick schlick(1.f, etaIn);
			float R = schlick.evaluate(normal, wi);
			float z = rand::rnd();
			if (z <= R)
			{ // Reflect
				wo = reflected;
				pdf = R;
				return R * this->evaluate(wo);
			}
			else
			{ // Refract
				wo = refracted;
				pdf = -(1.f - R);
				return (1.f - R) * this->evaluate(wo);
			}
		}
	protected:
		virtual float PDF(const vec3 &wo) const
		{
			return 0.f; // not used
		}
		virtual colorHDR evaluate(const vec3 &wo) const
		{
			return Ts / vec3::dot(wo, normal);
		}
	private:
		colorHDR Ts;
		float etaOut;
		float etaIn;
	};

	class MicrofacetReflection : BRDF
	{
	public:
		MicrofacetReflection(const colorHDR &color, const prim::HitInfo &info, float roughness) :
			BRDF(info), Rs(color), roughness(roughness)
		{
		}
		virtual colorHDR sample(vec3 &wo, float &pdf) const
		{
			float r1 = rand::rnd();
			float r2 = rand::rnd();
			float theta = atanf(roughness * sqrtf(r1) / sqrtf(1 - r1));
			float phi = 2 * M_PIf * r2;
			vec3 m;
			m.x = cosf(phi) * sinf(theta);
			m.z = sinf(phi) * sinf(theta);
			m.y = cosf(theta);
			transform::Onb onb(normal);
			wo = onb(m);
			pdf = this->PDF(wo);
			return this->evaluate(wo);
		}
	protected:
		virtual float PDF(const vec3 &wo) const
		{
			return vec3::dot(wo, normal) / M_PIf;
		}
		virtual colorHDR evaluate(const vec3 &wo) const
		{
			return Rs / M_PIf;
		}
	private:
		colorHDR Rs;
		float roughness;
		Fresnel *fresnel;
	};

}