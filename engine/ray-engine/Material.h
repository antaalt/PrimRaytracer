#pragma once

#include "Ray.h"
#include "Hitable.h"
#include "Onb.h"
#include "Types.h"
#include "Sampler.h"
#include "Texture.h"

namespace app {

	namespace prim {
		
		vec3 reflect(const vec3 &wi, const norm3 &normal);
		bool refract(vec3 &out, const vec3 &wi, const norm3 &normal, float eta);
		vec3 sampleUnitSphere(const rand::Sampler &sampler);
		vec3 sampleMicroFacet(const rand::Sampler &sampler, float roughness);

		enum class MaterialType {
			DIFFUSE,
			SPECULAR,
			DIELECTRIC,
			METAL,
			UNDEFINED_TYPE
		};

		class Material
		{
		public:
			using Ptr = Material*;

			virtual tracer::Ray scatter(const tracer::Ray &in, const prim::HitInfo &intersection, float &pdf) const = 0;
			virtual MaterialType type() const = 0;

			colorHDR color(float u = 0.f, float v = 0.f) const
			{
				if (colorTexture == nullptr)
					return albedo;
				else
					return albedo * colorTexture->texture2D(u, v);
			}

			void setColor(const colorHDR &color) { albedo = color; }
			void setTexture(Texture32 *texture) { colorTexture = texture; }

			Material() : colorTexture(nullptr) {}
		protected:
			colorHDR albedo;
			Texture32 *colorTexture;
		};

		class Diffuse : public Material {
		public:
			Diffuse(rand::Sampler sampler) : sampler(sampler) {}
			rand::Sampler sampler;
			virtual tracer::Ray scatter(const tracer::Ray &in, const prim::HitInfo &intersection, float &pdf) const
			{
				vec3 randomDirection = sampleUnitSphere(sampler);
				transform::Onb onb(intersection.normal);
				vec3 direction = onb.transform(randomDirection);
				//pdf = 1.f / M_PI;
				return tracer::Ray(intersection.point, direction);
			}
			virtual MaterialType type() const
			{
				return MaterialType::DIFFUSE;
			}
		};

		class Specular : public Material {
		public:
			Specular() {}
			virtual tracer::Ray scatter(const tracer::Ray &in, const prim::HitInfo &intersection, float &pdf) const
			{
				vec3 reflected = reflect(in.direction, intersection.normal);
				//pdf = 1.f;
				return tracer::Ray(intersection.point, reflected);
			}
			virtual MaterialType type() const
			{
				return MaterialType::SPECULAR;
			}
		};

		class Dielectric : public Material {
		public:
			Dielectric(float eta) : eta(eta) {}
			float eta;

			virtual tracer::Ray scatter(const tracer::Ray &in, const prim::HitInfo &intersection, float &pdf) const
			{
				bool inside = vec3::dot(in.direction, intersection.normal) > 0.f;
				float tmp_eta = 1.f / eta;
				vec3 refracted;
				bool tir = refract(refracted, in.direction, intersection.normal, tmp_eta); // TODO better fresnel & co
				//pdf = 1.f;
				return tracer::Ray(intersection.point, refracted);
			}
			virtual MaterialType type() const
			{
				return MaterialType::DIELECTRIC;
			}
		};

		class Metal : public Material {
		public:
			Metal(float roughness, rand::Sampler sampler) : roughness(roughness), sampler(sampler) {}
			float roughness;
			rand::Sampler sampler;

			virtual tracer::Ray scatter(const tracer::Ray &in, const prim::HitInfo &intersection, float &pdf) const
			{
				vec3 randomDirection = sampleMicroFacet(sampler, roughness);
				transform::Onb onb(intersection.normal);
				vec3 direction = onb.transform(randomDirection);
				pdf = 1.f; // TODO calculate pdf
				return tracer::Ray(intersection.point, direction);
			}
			virtual MaterialType type() const
			{
				return MaterialType::METAL;
			}
		};
	}
}