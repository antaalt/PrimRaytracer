#pragma once
#include "Config.h"
#include "Ray.h"
#include "Hitable.h"
#include "Random.h"
#include "transform.h"
#include "Fresnel.h"
#include "Sampling.h"

namespace raycore {

geometry::vec3f reflect(const geometry::vec3f &wi, const geometry::norm3f &normal);
bool refract(geometry::vec3f &out, const geometry::vec3f &wi, const geometry::norm3f &normal, float eta);

class BSDF
{
public:
	BSDF(const ComputedIntersection &info) : normal(info.normal), wi(info.direction) {}
	virtual geometry::color4f sample(geometry::vec3f &wo, float &pdf) const = 0;
protected:
	virtual float PDF(const geometry::vec3f &wo) const = 0;
	virtual geometry::color4f evaluate(const geometry::vec3f &wo) const = 0;
protected:
	geometry::norm3f normal;
	geometry::vec3f wi;
};

class BRDF : protected BSDF {
public:
	BRDF(const ComputedIntersection &info) : BSDF(info) {}
};

class BTDF : protected BSDF {
public:
	BTDF(const ComputedIntersection &info) : BSDF(info) {}

};

class LambertianReflection : BRDF
{
public:
	LambertianReflection(const geometry::color4f &color, const ComputedIntersection &info) : BRDF(info), Rd(color) {}
		
	virtual geometry::color4f sample(geometry::vec3f &wo, float &pdf) const
	{
		float r1 = Rand::sample<float>();
		float r2 = Rand::sample<float>();
		geometry::vec3f randomDirection = sample::unitHemisphere(r1, r2);
		transform::Onb onb(normal);
		wo = onb(randomDirection);
		pdf = this->PDF(wo);
		return this->evaluate(wo);
	}
protected:
	virtual float PDF(const geometry::vec3f &wo) const
	{
		return geometry::vec3f::dot(wo, geometry::vec3f(normal)) / geometry::pi<float>();
	}
	virtual geometry::color4f evaluate(const geometry::vec3f &wo) const
	{
		return Rd / geometry::pi<float>();
	}
private:
	geometry::color4f Rd;
};

class SpecularReflection : BRDF
{
public:
	SpecularReflection(const geometry::color4f &color, const ComputedIntersection &info) :
		BRDF(info), Rs(color)
	{
	}
	virtual geometry::color4f sample(geometry::vec3f &wo, float &pdf) const
	{
		wo = reflect(wi, normal);
		pdf = this->PDF(wo);
		return this->evaluate(wo);
	}
protected:
	virtual float PDF(const geometry::vec3f &wo) const
	{
		return 1.f;
	}
	virtual geometry::color4f evaluate(const geometry::vec3f &wo) const
	{
		return Rs / geometry::vec3f::dot(wo, geometry::vec3f(normal));
	}
private:
	geometry::color4f Rs;
};

class Specular : BSDF
{
public:
	Specular(const geometry::color4f &color, const ComputedIntersection &info, float etaOut, float etaIn) :
		BSDF(info), Ts(color), etaOut(etaOut), etaIn(etaIn)
	{
	}
	virtual geometry::color4f sample(geometry::vec3f &wo, float &pdf) const
	{
		geometry::norm3f nn = normal;

		float etar = 1.f / etaIn;
		if (geometry::vec3f::dot(wi, geometry::vec3f(normal)) >= 0.f)
		{
			etar = etaIn;
			nn = -normal;
		}

		geometry::vec3f refracted;
		geometry::vec3f reflected = reflect(wi, normal);
		if (refract(refracted, wi, nn, etar)) // TIR
		{
			wo = reflected;
			pdf = this->PDF(wo);
			return this->evaluate(wo);
		}
		Schlick schlick(1.f, etaIn);
		float R = schlick.evaluate(wi, normal);
		float z = Rand::sample<float>();
		if (z <= R)
		{ // Reflect
			wo = reflected;
			pdf = this->PDF(wo);
			return R * this->evaluate(wo);
		}
		else
		{ // Refract
			wo = refracted;
			pdf = this->PDF(wo);
			return (1.f - R) * this->evaluate(wo);
		}
	}
protected:
	virtual float PDF(const geometry::vec3f &wo) const
	{
		return 1.f;
	}
	virtual geometry::color4f evaluate(const geometry::vec3f &wo) const
	{
		return Ts / geometry::vec3f::dot(wo, geometry::vec3f(normal));
	}
private:
	geometry::color4f Ts;
	float etaOut;
	float etaIn;
};

class MicrofacetReflection : BRDF
{
public:
	MicrofacetReflection(const geometry::color4f &color, const ComputedIntersection &info, float roughness) :
		BRDF(info), Rs(color), roughness(roughness)
	{
	}
	virtual geometry::color4f sample(geometry::vec3f &wo, float &pdf) const
	{
		geometry::vec3f m = sample::unitMicrofacet(roughness, Rand::sample<float>(), Rand::sample<float>());
		transform::Onb onb(normal);
		wo = onb(m);
		pdf = this->PDF(wo);
		return this->evaluate(wo);
	}
protected:
	virtual float PDF(const geometry::vec3f &wo) const
	{
		return geometry::vec3f::dot(wo, geometry::vec3f(normal)) / geometry::pi<float>();
	}
	virtual geometry::color4f evaluate(const geometry::vec3f &wo) const
	{
		return Rs / geometry::pi<float>();
	}
private:
	geometry::color4f Rs;
	float roughness;
	Fresnel *fresnel;
};

}