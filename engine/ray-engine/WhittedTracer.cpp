#include "WhittedTracer.h"
#include "Material.h"

namespace app {
	namespace tracer {

		WhittedTracer::WhittedTracer()
		{
		}

		WhittedTracer::~WhittedTracer()
		{
		}
		Pixel WhittedTracer::castRay(const Ray & ray, const Accelerator::Ptr accelerator, const Options & options, unsigned int depth) const
		{
			if (depth > options.maxDepth)
				return miss(ray);
			prim::HitInfo info;
			if (!trace(ray, accelerator, info))
				return miss(ray);
				
			float pdf;
			Ray nextRay = info.material->scatter(ray, info, pdf);
			//Vector3 reflected = prim::reflect(ray.direction, info.normal);
			Pixel color = info.material->albedo * info.color * Vector3::dot(info.normal, -ray.direction);
			//Ray nextRay(info.point, reflected);
			return color + castRay(nextRay, accelerator, options, ++depth);
		}
		bool WhittedTracer::trace(const Ray & ray, const Accelerator::Ptr accelerator, prim::HitInfo & info) const
		{
			return accelerator->intersect(ray, info);
		}
		Pixel WhittedTracer::miss(const Ray & ray) const
		{
			//return Pixel((ray.direction.x + 1.f) / 2.f, (ray.direction.y + 1.f) / 2.f, (ray.direction.z + 1.f) / 2.f, 1.f);
			return Pixel(Vector3::dot(ray.direction, Vector3(0.f, 1.f, 0.f)));
		}
	}
}

