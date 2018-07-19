#pragma once

#include "Ray.h"
#include "Hitable.h"
#include "Onb.h"
#include "Types.h"
#include "Texture.h"

#include <cmath>

namespace Application::RayTracer {

	struct Sampler {
		float r1() const;
		float r2() const;
	};

	Vector3 reflect(const Vector3 &wi, const Normal &normal);
	Vector3 refract(const Vector3 &wi, const Normal &normal, float eta);
	Vector3 sampleUnitSphere(const Sampler &sampler);
	Vector3 sampleMicroFacet(const Sampler &sampler, float roughness);

	enum class MaterialType {
		LAMBERTIAN,
		SPECULAR,
		TRANSMISSION,
		MICROFACET,
		UNDEFINED_TYPE
	};

	class Material
	{
	public:
		Color32 albedo;
		Texture *colorTexture;
			
		virtual Ray scatter(const Ray &in, const Intersection &intersection, float &pdf) const = 0;
		virtual MaterialType type() const = 0;

		Material() : colorTexture(nullptr) {}
	};

	class Lambertian : public Material {
	public:
		Sampler sampler;
		virtual Ray scatter(const Ray &in, const Intersection &intersection, float &pdf)
		{
			Vector3 randomDirection = sampleUnitSphere(sampler);
			Onb onb(intersection.normal);
			Vector3 direction = onb.transform(randomDirection);
			//pdf = 1.f / M_PI;
			return Ray(intersection.point, direction);
		}
		virtual MaterialType type() const
		{
			return MaterialType::LAMBERTIAN;
		}
	};

	class Specular : public Material {
	public:
		virtual Ray scatter(const Ray &in, const Intersection &intersection, float &pdf)
		{
			Vector3 reflected = reflect(in.direction, intersection.normal);
			//pdf = 1.f;
			return Ray(intersection.point, reflected);
		}
		virtual MaterialType type() const
		{
			return MaterialType::SPECULAR;
		}
	};

	class Transmission : public Material {
	public:
		float eta;

		virtual Ray scatter(const Ray &in, const Intersection &intersection, float &pdf)
		{
			bool inside = dot(in.direction, intersection.normal) > 0.f;
			float tmp_eta = 1.f / eta;
			Vector3 refracted = refract(in.direction, intersection.normal, tmp_eta); // TODO better fresnel & co
			//pdf = 1.f;
			return Ray(intersection.point, refracted);
		}
		virtual MaterialType type() const
		{
			return MaterialType::TRANSMISSION;
		}
	};

	class MicroFacet : public Material {
	public:
		Vector3 roughness;
		Sampler sampler;

		virtual Ray scatter(const Ray &in, const Intersection &intersection, float &pdf)
		{
			Vector3 randomDirection = sampleMicroFacet(sampler,  0.5f);
			Onb onb(intersection.normal);
			Vector3 direction = onb.transform(randomDirection);
			pdf = 1.f; // TODO calculate pdf
			return Ray(intersection.point, direction);
		}
		virtual MaterialType type() const
		{
			return MaterialType::MICROFACET;
		}
	};
}