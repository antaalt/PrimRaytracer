#include "PathTracer.h"
#include "Hitable.h"
#include "Material.h"
#include "Random.h"
#include "Light.h"

namespace raycore {

	namespace tracer {

		PathTracer::PathTracer()
		{
		}

		colorHDR sampleLights(const prim::HitInfo &info, const prim::Scene& scene)
		{
			colorHDR radiance(0.f);
			const prim::LightDistribution *lightDistribution = scene.getLightDistribution();
			/*for (size_t iLight = 0; iLight < lightDistribution.; iLight++)
			{
				const Light *l = accelerator->getLight(iLight);
				LightInfo linfo;
				if (l->sample(info, accelerator, linfo))
				{
					radiance = radiance + linfo.color * std::abs(dot(linfo.sample, info.normal));
				}
			}*/
			return lightDistribution->sampleOneLight(info, scene);
		}

		colorHDR PathTracer::castRay(const Ray & ray, const prim::Scene& scene, unsigned int depth) const
		{
			if (depth == 0)
				return colorHDR(0.f);
			prim::HitInfo info;
			if (!trace(ray, scene, info))
				return miss(ray);

			// if material is emissive, return emission
			colorHDR emission(0.f);

			// Scatter ray
			float pdf;
			vec3 wo;
			prim::BSDFType type;
			colorHDR brdf = info.material->sample(ray, info, wo, pdf, type) * info.color;
			colorHDR reflectance = brdf * dot(wo, info.normal) / pdf;

			// Sample light (depending on type)
			colorHDR lightRadiance(0.f);
			if (!(type & prim::BSDF_SPECULAR))
			{
				lightRadiance = reflectance * sampleLights(info, scene);
			}

			// Russian roulette
			if (depth < (RAY_DEFAULT_DEPTH / 2))
			{
				float probability = max(reflectance.r, max(reflectance.g, reflectance.b));
				if (probability < rand::rnd())
					return colorHDR(0.f);
				reflectance = reflectance / probability;
			}
			//return colorHDR(info.normal.x, info.normal.y, info.normal.z, 1.f);

			colorHDR ret = lightRadiance + reflectance * castRay(Ray(info.point, wo), scene, depth - 1);
			return ret;
		}

		bool PathTracer::trace(const Ray & ray, const prim::Scene& scene, prim::HitInfo &info) const
		{
			return scene.intersect(ray, info);
		}
		colorHDR PathTracer::miss(const Ray & ray) const
		{
			return colorHDR(0.f);
		}
	}
}