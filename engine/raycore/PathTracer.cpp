#include "PathTracer.h"
#include "Hitable.h"
#include "Material.h"
#include "Mathematic.h"
#include "Sampler.h"

namespace raycore {

	namespace tracer {

		PathTracer::PathTracer() :
			sampleCount(0)
		{
		}

		void PathTracer::castRay(Pixel & pixel, const Ray & ray, const Accelerator * accelerator) const
		{
			// sub pixel jitter
			float r1 = rand::Random::get(0.f, 1.f);
			float r2 = rand::Random::get(0.f, 1.f);
			vec3 jittered = ray.direction;
			Ray jitteredRay(ray.origin, jittered, ray.type, ray.tmin, ray.tmax);
			colorHDR p = this->castRay(jitteredRay, accelerator, 0);
			pixel = lerp(colorHDR(pixel), p, 1.f / (sampleCount + 1.f));
		}

		colorHDR PathTracer::castRay(const Ray & ray, const Accelerator* accelerator, unsigned int depth) const
		{
			if (depth > MAX_DEPTH)
				return colorHDR(1.f);
			prim::HitInfo info;
			if (!trace(ray, accelerator, info))
				return miss(ray);
			// 0. fetch material and compute nextRay
			float pdf;
			Ray nextRay = info.material->scatter(ray, info, pdf);

			// 1. first add emission
			// 2. Russian Roulette
			colorHDR reflectance = info.material->color(info.texcoord.x, info.texcoord.y) * info.color;
			float probability = max(reflectance.x, max(reflectance.y, reflectance.z));
			if (probability < rand::Random::get(0.f, 1.f))
				return colorHDR(0.f); // return emission
			reflectance = reflectance / probability;
			switch (info.material->type())
			{
			case prim::MaterialType::DIFFUSE:
				// emission
				return colorHDR(0.f) + reflectance * castRay(nextRay, accelerator, depth+1);
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
			return colorHDR(20.f);
		}
		void PathTracer::postProcess()
		{
			sampleCount++;
		}
		void PathTracer::reset()
		{
			sampleCount = 0;
		}
	}
}