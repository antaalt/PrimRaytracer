#pragma once
#include "Ray.h"
#include "Hitable.h"
#include "Accelerator.h"
#include "PixelBuffer.h"

#define MAX_DEPTH 10
#define BACKGROUND_COLOR colorHDR(0.1f)

namespace raycore {

	namespace tracer {

		class Tracer
		{
		public:
			using Ptr = Tracer*;
			Tracer();
			virtual ~Tracer();

			virtual Pixel castRay(const Ray &ray, const Accelerator* accelerator, unsigned int depth = 0) const = 0;

			virtual bool trace(const Ray &ray, const Accelerator* accelerator, prim::HitInfo &info) const = 0;

			virtual Pixel miss(const Ray &ray) const = 0;


		};
	}
}


