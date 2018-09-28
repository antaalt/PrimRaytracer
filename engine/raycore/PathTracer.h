#pragma once
#include "Tracer.h"

namespace raycore {
	namespace tracer {

		class PathTracer : public Tracer
		{
		public:
			PathTracer();
			virtual void shade(Pixel &pixel, const Ray &ray, const Accelerator* accelerator, unsigned int depth = MAX_DEPTH) const;
			virtual void postProcess();
			virtual void reset();

		private:
			virtual colorHDR castRay(const Ray &ray, const Accelerator* accelerator, unsigned int depth = 0) const;
			virtual bool trace(const Ray &ray, const Accelerator* accelerator, prim::HitInfo &info) const;
			virtual colorHDR miss(const Ray &ray) const;

		private:
			unsigned int sampleCount;
		};
	}
}