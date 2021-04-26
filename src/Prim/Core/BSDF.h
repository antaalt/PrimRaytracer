#pragma once
#include "../Core/Geometry.h"
#include "../Core/Ray.h"
#include "../Core/Hitable.h"
#include "../Core/Transform.h"
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
		float r1 = random();
		float r2 = random();
		// Disk sampling projected on hemisphere. More efficient and give a cosine weighted distribution
		// from Global Illuination Compendium item 35
		// http://www.rorydriscoll.com/2009/01/07/better-sampling/
		vec3f randomDirection = vec3f(
			polar<float>(2.f * pi<float> *r2, sqrt(r1)).cartesian(), 
			sqrt(1.f - r1)
		);
		Onb onb(normal);
		return onb(randomDirection);
	}
	float pdf(const vec3f &wo, const norm3f &normal) const override
	{
		// We can remove pi as it cancel itself with evaluate pi
		return vec3f::dot(wo, vec3f(normal));// / pi<float>();
	}
	color4f evaluate(const color4f &albedo, const vec3f &wo, const norm3f &normal) const override
	{
		return albedo;// / pi<float>();
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
		return albedo / vec3f::dot(wo, vec3f(normal));
	}
};


struct Specular : BSDF {
	Specular(float eta) : m_eta(eta) {}
	vec3f scatter(const vec3f &wi, const norm3f &normal) const override
	{
		norm3f nn = normal;

		float etar = 1.f / m_eta;
		if (vec3f::dot(wi, vec3f(normal)) >= 0.f)
		{
			etar = m_eta;
			nn = -normal;
		}

		vec3f refracted;
		vec3f reflected = reflect(wi, normal);
		if (refract(refracted, wi, nn, etar)) // TIR
			return reflected;

		Schlick schlick(1.f, m_eta);
		float R = schlick.evaluate(wi, normal);
		float z = random();
		if (z <= R)
			return reflected;
		else
			return refracted;
	}
	float pdf(const vec3f &wo, const norm3f &normal) const override
	{
		return 1.f;
	}
	color4f evaluate(const color4f &albedo, const vec3f &wo, const norm3f &normal) const override
	{
		return albedo / vec3f::dot(wo, vec3f(normal));
	}
private:
	float m_eta;
};

struct MicrofacetReflection : BRDF
{
	MicrofacetReflection(float roughness) : m_roughness(roughness) {}

	vec3f scatter(const vec3f &wi, const norm3f &normal) const override
	{
		float r1 = random();
		float r2 = random();
		vec3f m = spherical<float>(
			arctan(m_roughness * sqrt(r1) / sqrt(1.f - r1)),
			2.f * pi<float> *r2, 
			1.f
		).cartesian();
		Onb onb(normal);
		return onb(m);
	}
	float pdf(const vec3f &wo, const norm3f &normal) const override
	{
		return vec3f::dot(wo, vec3f(normal)) / pi<float>.radian();
	}
	color4f evaluate(const color4f &albedo, const vec3f &wo, const norm3f &normal) const override
	{
		return albedo / pi<float>.radian();
	}
private:
	float m_roughness;
};

}