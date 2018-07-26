#pragma once
#include "Ray.h"
#include "Hitable.h"
#include "Texture.h"
#include "Accelerator.h"

namespace Application::RayTracer {

	class Root;
	

	struct Options {
		unsigned int maxDepth;
		Pixel backgroundColor;
	};

	class Tracer
	{
	public:
		using Ptr = Tracer*;
		Tracer();
		virtual ~Tracer();

		virtual Pixel castRay(const Ray &ray, const Accelerator &accelerator, const Options &options, unsigned int depth = 0) const = 0;

		virtual bool trace(const Ray &ray, const Accelerator &accelerator, Intersection &intersection) const = 0;
		

	};
}


