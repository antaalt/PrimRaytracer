#pragma once
#include "Config.h"
#include "Ray.h"
#include "Hitable.h"
#include "Random.h"
#include "Transform.h"
#include "Fresnel.h"

namespace prim {

struct BSDF {
	virtual vec3f scatter(const vec3f &wi, const norm3f &normal) const = 0; // add const
	virtual float pdf(const vec3f &wo, const norm3f &normal) const = 0; // add const
	virtual color4f evaluate(const color4f &albedo, const vec3f &wo, const norm3f &normal) const = 0; // add const
};

struct BRDF : BSDF {

};

struct BTDF : BSDF {

};

struct LambertianReflection : BRDF {
	vec3f scatter(const vec3f &wi, const norm3f &normal) const override
	{
		float r1 = Rand::sample<float>();
		float r2 = Rand::sample<float>();
		geometry::vec3f randomDirection = sample::unitHemisphere(r1, r2);
		Onb onb(normal);
		return onb(randomDirection);
	}
	float pdf(const vec3f &wo, const norm3f &normal) const override
	{
		return geometry::vec3f::dot(wo, geometry::vec3f(normal)) / geometry::pi<float>();
	}
	color4f evaluate(const color4f &albedo, const vec3f &wo, const norm3f &normal) const override
	{
		return albedo / geometry::pi<float>();
	}
};

struct SpecularReflection : BRDF {
	vec3f scatter(const vec3f &wi, const norm3f &normal) const override
	{
		return reflect(wi, normal);
	}
	float pdf(const vec3f &wo, const norm3f &normal) const override
	{
		return 1.f;
	}
	color4f evaluate(const color4f &albedo, const vec3f &wo, const norm3f &normal) const override
	{
		return albedo / geometry::vec3f::dot(wo, geometry::vec3f(normal));
	}
};


struct Specular : BSDF {
	Specular(float eta) : m_eta(eta) {}
	vec3f scatter(const vec3f &wi, const norm3f &normal) const override
	{
		geometry::norm3f nn = normal;

		float etar = 1.f / m_eta;
		if (geometry::vec3f::dot(wi, geometry::vec3f(normal)) >= 0.f)
		{
			etar = m_eta;
			nn = -normal;
		}

		geometry::vec3f refracted;
		geometry::vec3f reflected = reflect(wi, normal);
		if (refract(refracted, wi, nn, etar)) // TIR
			return reflected;

		Schlick schlick(1.f, m_eta);
		float R = schlick.evaluate(wi, normal);
		float z = Rand::sample<float>();
		if (z <= R)
			return reflected;
		else
			return refracted;
	}
	float pdf(const vec3f &wo, const norm3f &normal) const override
	{
		return 1.f;
	}
	geometry::color4f evaluate(const color4f &albedo, const vec3f &wo, const norm3f &normal) const override
	{
		return albedo / geometry::vec3f::dot(wo, geometry::vec3f(normal));
	}
private:
	float m_eta;
};

struct MicrofacetReflection : BRDF
{
	MicrofacetReflection(float roughness) : m_roughness(roughness) {}

	vec3f scatter(const vec3f &wi, const norm3f &normal) const override
	{
		geometry::vec3f m = sample::unitMicrofacet(m_roughness, Rand::sample<float>(), Rand::sample<float>());
		Onb onb(normal);
		return onb(m);
	}
	float pdf(const vec3f &wo, const norm3f &normal) const override
	{
		return geometry::vec3f::dot(wo, geometry::vec3f(normal)) / geometry::pi<float>();
	}
	geometry::color4f evaluate(const color4f &albedo, const vec3f &wo, const norm3f &normal) const override
	{
		return albedo / geometry::pi<float>();
	}
private:
	float m_roughness;
};

}