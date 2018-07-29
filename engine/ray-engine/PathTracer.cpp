#include "PathTracer.h"
#include "Hitable.h"
#include "Material.h"


namespace app {

	namespace tracer {

		PathTracer::PathTracer()
		{
		}


		PathTracer::~PathTracer()
		{
		}

		Pixel PathTracer::castRay(const Ray & ray, const Accelerator::Ptr accelerator, const Options & options, unsigned int depth) const
		{
			if (depth > options.maxDepth)
				return options.backgroundColor;
			prim::HitInfo info;
			if (!trace(ray, accelerator, info))
				return miss(ray);
			prim::Material::Ptr mat = info.material;
			float pdf;
			Ray newRay = mat->scatter(ray, info, pdf);

			Pixel color;
			ColorHDR directLight;
			ColorHDR indirectLight;
			switch (mat->type())
			{
			case prim::MaterialType::DIFFUSE:
				// TODO compute direct lighting
				indirectLight = castRay(newRay, accelerator, options, depth++);
				break;
			case prim::MaterialType::SPECULAR:
				break;
			case prim::MaterialType::DIELECTRIC:
				break;
			case prim::MaterialType::METAL:
				break;
			default:
				break;
			}

			color = directLight + indirectLight; // TODO pdf & BRDF
			return color;
		}

		bool PathTracer::trace(const Ray & ray, const Accelerator::Ptr accelerator, prim::HitInfo &info) const
		{
			return accelerator->intersect(ray, info);
		}
		Pixel PathTracer::miss(const Ray & ray) const
		{
			return Pixel();
		}
	}
}