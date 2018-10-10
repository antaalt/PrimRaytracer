#pragma once

#include "Ray.h"
#include "Hitable.h"
#include "Onb.h"
#include "Types.h"
#include "Random.h"
#include "Texture.h"

namespace raycore {

	namespace prim {
		
		vec3 reflect(const vec3 &wi, const norm3 &normal);
		bool refract(vec3 &out, const vec3 &wi, const norm3 &normal, float eta);
		vec3 sampleUnitSphere();
		vec3 sampleMicroFacet(float roughness);

		enum BxDFType {
			BSDF_REFLECTION,
			BSDF_DIFFUSE,
			BSDF_SPECULAR,
			BSDF_TRANSMISSION,
			BSDF_GLOSSY,
			NB_BSDF
		};

		enum TextureType {
			COLOR_TEXTURE,
			NORMAL_TEXTURE,
			BUMP_TEXTURE,
			NB_TEXTURES_TYPE
		};

		class Material
		{
		public:
			Material() {}

			//virtual tracer::Ray scatter(const tracer::Ray &in, const prim::HitInfo &intersection, float &pdf) const = 0;
			virtual colorHDR sample(const tracer::Ray &in, const prim::HitInfo &info, vec3 &wo, float &pdf) const = 0;

		protected:
			virtual float pdf(const vec3 &wi, const vec3 &wo, const prim::HitInfo &info) const = 0;
			virtual colorHDR brdf(float u, float v) const = 0;
			Texture32 *texture[NB_TEXTURES_TYPE];
		};

		class Diffuse : public Material {
		public:
			Diffuse(Texture32 *colorTexture, colorHDR color) : color(color) { texture[COLOR_TEXTURE] = colorTexture; }

			virtual colorHDR sample(const tracer::Ray &in, const prim::HitInfo &info, vec3 &wo, float &pdf) const
			{
				vec3 randomDirection = sampleUnitSphere();
				transform::Onb onb(info.normal);
				wo = onb.transform(randomDirection);
				pdf = this->pdf(in.direction, wo, info);
				return this->brdf(info.texcoord.x, info.texcoord.y);
			}
		private:
			virtual float pdf(const vec3 &wi, const vec3 &wo, const prim::HitInfo &intersection) const
			{
				return vec3::dot(wo, intersection.normal) / M_PIf;
			}
			virtual colorHDR brdf(float u, float v) const
			{
				if (texture[COLOR_TEXTURE] != nullptr)
					return (color * texture[COLOR_TEXTURE]->texture2D(u, v)) / M_PIf;
				else
					return color / M_PIf;
			}
		private:
			colorHDR color;
		};

		class Glossy : public Material {
		public:
			Glossy(Texture32 *colorTexture, colorHDR color) : color(color) { texture[COLOR_TEXTURE] = colorTexture; }

			virtual colorHDR sample(const tracer::Ray &in, const prim::HitInfo &info, vec3 &wo, float &pdf) const
			{
				wo = reflect(in.direction, info.normal);
				pdf = this->pdf(in.direction, wo, info);
				return this->brdf(info.texcoord.x, info.texcoord.y);
			}
		private:
			virtual float pdf(const vec3 &wi, const vec3 &wo, const prim::HitInfo &intersection) const
			{
				return 1.f;
			}
			virtual colorHDR brdf(float u, float v) const
			{
				if (texture[COLOR_TEXTURE] != nullptr)
					return (color * texture[COLOR_TEXTURE]->texture2D(u, v)) / M_PIf;
				else
					return color;
			}
		private:
			colorHDR color;
		};


		class Specular : public Material {
		public:
			Specular(Texture32 *colorTexture, colorHDR color) : color(color) { texture[COLOR_TEXTURE] = colorTexture; }

			virtual colorHDR sample(const tracer::Ray &in, const prim::HitInfo &info, vec3 &wo, float &pdf) const
			{
				wo = reflect(in.direction, info.normal);
				pdf = this->pdf(in.direction, wo, info);
				return this->brdf(info.texcoord.x, info.texcoord.y);
			}
		private:
			virtual float pdf(const vec3 &wi, const vec3 &wo, const prim::HitInfo &intersection) const
			{
				return 1.f;
			}
			virtual colorHDR brdf(float u, float v) const
			{
				if (texture[COLOR_TEXTURE] != nullptr)
					return (color * texture[COLOR_TEXTURE]->texture2D(u, v)) / M_PIf;
				else
					return color;
			}
		private:
			colorHDR color;
		};

		/*class Dielectric : public Material {
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
			Metal(float roughness) : roughness(roughness) {}
			float roughness;

			virtual tracer::Ray scatter(const tracer::Ray &in, const prim::HitInfo &intersection, float &pdf) const
			{
				vec3 randomDirection = sampleMicroFacet(roughness);
				transform::Onb onb(intersection.normal);
				vec3 direction = onb.transform(randomDirection);
				pdf = 1.f; // TODO calculate pdf
				return tracer::Ray(intersection.point, direction);
			}
			virtual MaterialType type() const
			{
				return MaterialType::METAL;
			}
		};*/
	}
}