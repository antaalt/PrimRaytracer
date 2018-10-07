#pragma once
#include "Tracer.h"

namespace raycore {
	namespace tracer {

		class WhittedTracer : public Tracer
		{
		public:
			WhittedTracer();
			~WhittedTracer();

			virtual colorHDR castRay(const Ray &ray, const Accelerator* accelerator, unsigned int depth = Config::maxDepth) const;

		private:
			virtual bool trace(const Ray &ray, const Accelerator* accelerator, prim::HitInfo & info) const;
			virtual colorHDR miss(const Ray &ray) const;

		};
	}
}

