#pragma once
#include "Tracer.h"

namespace raycore {
	namespace tracer {

		class PathTracer : public Tracer
		{
		public:
			PathTracer();
			virtual colorHDR castRay(const Ray &ray, const Accelerator* accelerator, unsigned int depth = MAX_DEPTH) const;

		private:
			virtual bool trace(const Ray &ray, const Accelerator* accelerator, prim::HitInfo &info) const;
			virtual colorHDR miss(const Ray &ray) const;
		};
	}
}