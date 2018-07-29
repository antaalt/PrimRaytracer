#pragma once
#include "Tracer.h"

namespace app {
	namespace tracer {

		class WhittedTracer : public Tracer
		{
		public:
			WhittedTracer();
			~WhittedTracer();
			virtual Pixel castRay(const Ray &ray, const Accelerator::Ptr accelerator, const Options &options, unsigned int depth = 0) const;

			virtual bool trace(const Ray &ray, const Accelerator::Ptr accelerator, prim::HitInfo & info) const;
			
			virtual Pixel miss(const Ray &ray) const;

		};
	}
}

