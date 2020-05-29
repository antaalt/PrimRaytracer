#pragma once
#include "Tracer.h"

namespace prim {
namespace tracer {

class PathTracer : public Tracer
{
public:
	PathTracer();
	virtual geometry::color4f castRay(const Ray &ray, const Scene& scene, unsigned int depth = RAY_DEFAULT_DEPTH) const;

private:
	virtual geometry::color4f miss(const Ray &ray) const;
};

}
}