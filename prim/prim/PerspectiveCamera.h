#pragma once

#include "Camera.h"

namespace prim {

struct PerspectiveCamera : public Camera {
	mat4f perspective;
	float hFov;

	Ray generateRay(const RaySampler::Type &screenPos) const override;
};

}