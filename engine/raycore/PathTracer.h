#pragma once
#include "Tracer.h"

namespace raycore {
	namespace tracer {

		class PathTracer : public Tracer
		{
		public:
			PathTracer();
			~PathTracer();
			virtual Pixel castRay(const Ray &ray, const Accelerator::Ptr accelerator, unsigned int depth = 0) const;

			virtual bool trace(const Ray &ray, const Accelerator::Ptr accelerator, prim::HitInfo &info) const;

			virtual Pixel miss(const Ray &ray) const;
		};
	}
}