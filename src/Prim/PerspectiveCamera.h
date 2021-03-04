#pragma once

#include "Camera.h"

namespace prim {

struct PerspectiveCamera : public Camera {
	PerspectiveCamera();

	mat4f perspective;
	mat4f inverse; // inverse perspective cache
	float hFov;
	float lensRadius;
	float focalDistance;

	Ray generateRay(const RaySampler::Type &screenPos) const override;
};

}