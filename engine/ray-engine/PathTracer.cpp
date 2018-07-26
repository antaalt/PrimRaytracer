#include "PathTracer.h"
#include "Hitable.h"
#include "Material.h"


namespace Application::RayTracer {

	PathTracer::PathTracer()
	{
	}

	PathTracer::PathTracer(unsigned int maxDepth) : max_depth(maxDepth)
	{
	}


	PathTracer::~PathTracer()
	{
	}

	Pixel miss()
	{
		return Pixel();
	}

	Pixel PathTracer::castRay(const Ray & ray, const Accelerator & accelerator, const Options & options, unsigned int depth) const
	{
		if (depth > options.maxDepth)
			return options.backgroundColor;
		Intersection intersection;
		if (!trace(ray, accelerator, intersection))
			return miss();

		float pdf;
		Ray newRay = intersection.material->scatter(ray, intersection, pdf);

		Color32 color;
		Color32 directLight;
		Color32 indirectLight;
		switch (intersection.material->type())
		{
		case MaterialType::LAMBERTIAN:
			// TODO compute direct lighting
			indirectLight = castRay(newRay, accelerator, options, depth++).color32();
			break;
		case MaterialType::SPECULAR:
			break;
		case MaterialType::TRANSMISSION:
			break;
		case MaterialType::MICROFACET:
			break;
		default:
			break;
		}

		color = directLight + indirectLight; // TODO pdf & BRDF
		return color;
	}

	bool PathTracer::trace(const Ray & ray, const Accelerator & accelerator, Intersection &intersection) const
	{
		return accelerator.intersect(ray, intersection);
	}

}