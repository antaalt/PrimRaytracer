#pragma once
#include "Tracer.h"

namespace prim {

class PathTracer : public Tracer
{
public:
	PathTracer(uint32_t maxDepth);

	color4f render(const Ray &ray, const Scene& scene) const override;
private:
	uint32_t m_maxDepth;
};

}