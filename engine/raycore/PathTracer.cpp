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

		colorHDR sampleLights(const prim::HitInfo &info, const Accelerator* accelerator)
		{
			colorHDR radiance(0.f);
			for (size_t iLight = 0; iLight < accelerator->getLightsCount(); iLight++)
			{
				const Light *l = accelerator->getLight(iLight);
				LightInfo linfo;
				if (l->sample(info, accelerator, linfo))
				{
					radiance = radiance + linfo.color * std::abs(dot(linfo.sample, info.normal));
				}
			}
			return radiance;
		}

		colorHDR PathTracer::castRay(const Ray & ray, const Accelerator* accelerator, unsigned int depth) const
		{
			if (depth == 0)
				return colorHDR(0.f);
			prim::HitInfo info;
			if (!trace(ray, accelerator, info))
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
				lightRadiance = reflectance * sampleLights(info, accelerator);
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

			colorHDR ret = lightRadiance + reflectance * castRay(Ray(info.point, wo), accelerator, depth - 1);
			if (hasNan(ret))
				__debugbreak();
			return ret;
		}

		bool PathTracer::trace(const Ray & ray, const Accelerator* accelerator, prim::HitInfo &info) const
		{
			return accelerator->intersect(ray, info);
		}
		colorHDR PathTracer::miss(const Ray & ray) const
		{
			return colorHDR(0.9f);
		}
	}
}