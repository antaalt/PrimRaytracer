#pragma once
#include "Camera.h"

namespace prim {

struct PinholeCamera : public Camera
{
	norm3f u, v, w;
	point3f eye;
	float hFov;
	float aperture, focalDist;

	Ray generateRay(const RaySampler::Type &screenPos) const override;
};

}

