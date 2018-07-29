#pragma once
#include "Ray.h"
#include "Hitable.h"
#include "Accelerator.h"
#include "PixelBuffer.h"
namespace app {

	namespace tracer {

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

			virtual Pixel castRay(const Ray &ray, const Accelerator::Ptr accelerator, const Options &options, unsigned int depth = 0) const = 0;

			virtual bool trace(const Ray &ray, const Accelerator::Ptr accelerator, prim::HitInfo &info) const = 0;

			virtual Pixel miss(const Ray &ray) const = 0;


		};
	}
}


