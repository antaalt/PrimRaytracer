#pragma once
#include "../Core/Tracer.h"

namespace prim {

class WhittedTracer : public Tracer {
public:
	WhittedTracer(uint32_t maxDepth);

	geometry::color4f render(const Ray &ray, const Scene &scene) const override;

private:
	uint32_t m_maxDepth;
};

}

