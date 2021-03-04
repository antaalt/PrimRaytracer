#pragma once
#include "Config.h"

#include "Ray.h"
#include "Random.h"
#include "Transform.h"


namespace prim {

using vec2u = vec2<uint32_t>;

struct RaySampler {
	using Type = vec2f;
	virtual Type operator()(vec2u position, vec2u size) = 0;
};

struct LinearRaySampler : public RaySampler {
	RaySampler::Type operator()(vec2u position, vec2u size) override;
};

struct RandomRaySampler : public RaySampler {
	RaySampler::Type operator()(vec2u position, vec2u size) override;
};

struct Camera {
	Transform transform;

	virtual Ray generateRay(const RaySampler::Type &screenPos) const = 0;
};

}
