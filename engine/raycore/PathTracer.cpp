#include "PathTracer.h"
#include "Hitable.h"
#include "Material.h"
#include "Mathematic.h"
#include "Random.h"
#include "Light.h"

namespace raycore {

	namespace tracer {

		PathTracer::PathTracer()
		{
		}

		colorHDR PathTracer::castRay(const Ray & ray, const Accelerator* accelerator, unsigned int depth) const
		{
			if (depth == 0)
				return colorHDR(1.f);
			prim::HitInfo info;
			if (!trace(ray, accelerator, info))
				return miss(ray);
			// 0. fetch material and compute nextRay
			float pdf;
			Ray nextRay = info.material->scatter(ray, info, pdf);
			colorHDR emission(0.f);

			// 1. first add emission
			// 2. Russian Roulette
			colorHDR reflectance = info.material->color(info.texcoord.x, info.texcoord.y) * info.color;
			float probability = max(reflectance.x, max(reflectance.y, reflectance.z));
			if (probability < rand::rnd())
				return emission;
			reflectance = reflectance / probability;
			switch (info.material->type())
			{
			case prim::MaterialType::DIFFUSE:
			{
				colorHDR radiance(0.f);
				for (size_t iLight = 0; iLight < accelerator->getLightsCount(); iLight++)
				{
					const Light *l = accelerator->getLight(iLight);
					LightInfo linfo;
					if (l->sample(info, accelerator, linfo))
					{
						radiance = radiance + reflectance * linfo.color * std::abs(vec3::dot(linfo.sample, info.normal));
					}
				}
				return emission + radiance + reflectance * castRay(nextRay, accelerator, depth - 1);
			}
			case prim::MaterialType::SPECULAR:
			case prim::MaterialType::DIELECTRIC:
			case prim::MaterialType::METAL:
			default:
				throw std::runtime_error("not supported");
				break;
			}
			return reflectance;
		}

		bool PathTracer::trace(const Ray & ray, const Accelerator* accelerator, prim::HitInfo &info) const
		{
			return accelerator->intersect(ray, info);
		}
		colorHDR PathTracer::miss(const Ray & ray) const
		{
			return colorHDR(1.f);
		}
	}
}