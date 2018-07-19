#pragma once
#include "Tracer.h"

namespace Application::RayTracer {

	class PathTracer : public Tracer
	{
	public:
		PathTracer();
		~PathTracer();
		virtual Pixel castRay(const Ray &ray, const Accelerator &accelerator, const Options &options, unsigned int depth = 0) const;

		virtual bool trace(const Ray &ray, const Accelerator &accelerator, Intersection &intersection) const;

		unsigned int max_depth;
	};

}