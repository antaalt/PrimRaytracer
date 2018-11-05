#pragma once
#include "Tracer.h"

namespace raycore {
	namespace tracer {

		class WhittedTracer : public Tracer
		{
		public:
			WhittedTracer();
			~WhittedTracer();

			virtual colorHDR castRay(const Ray &ray, const prim::Scene &scene, unsigned int depth = RAY_DEFAULT_DEPTH) const;

		private:
			virtual bool trace(const Ray &ray, const prim::Scene &scene, prim::HitInfo & info) const;
			virtual colorHDR miss(const Ray &ray) const;

		};
	}
}

