#pragma once

#include "Geometry.h"
#include "Ray.h"
#include "Transform.h"


namespace prim {

struct RaySampler {
	using Type = vec2f;
	virtual Type operator()(vec2u position, vec2u size) = 0;
};

struct LinearRaySampler : public RaySampler {
	RaySampler::Type operator()(vec2u position, vec2u size) override;
};

struct RandomRaySampler : public RaySampler {
	RandomRaySampler(uint32_t sample);
	RaySampler::Type operator()(vec2u position, vec2u size) override;
	uint32_t iSample;
};

struct Camera {
	Transform transform;

	virtual Ray generateRay(const RaySampler::Type &screenPos) const = 0;
};

}
