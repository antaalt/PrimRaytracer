#pragma once
#include "Tracer.h"

namespace raycore {
namespace tracer {

class WhittedTracer : public Tracer
{
public:
	WhittedTracer();
	~WhittedTracer();

	virtual geometry::color4f castRay(const Ray &ray, const prim::Scene &scene, unsigned int depth = RAY_DEFAULT_DEPTH) const;

private:
	virtual bool trace(const Ray &ray, const prim::Scene &scene, prim::HitInfo & info) const;
	virtual geometry::color4f miss(const Ray &ray) const;

};

}
}

