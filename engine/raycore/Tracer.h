#pragma once
#include "Ray.h"
#include "Hitable.h"
#include "Accelerator.h"
#include "PixelBuffer.h"

//#define MAX_DEPTH 10
#define BACKGROUND_COLOR colorHDR(0.1f)

namespace raycore {

	namespace tracer {

		class Tracer
		{
		public:
			virtual colorHDR castRay(const Ray &ray, const Accelerator* accelerator, unsigned int depth = Config::maxDepth) const = 0;
			virtual void postProcess() {};
			virtual void reset() {};

		private:
			virtual bool trace(const Ray &ray, const Accelerator* accelerator, prim::HitInfo &info) const = 0;
			virtual colorHDR miss(const Ray &ray) const = 0;

		};
	}
}


