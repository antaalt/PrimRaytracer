#pragma once

#include "Camera.h"

namespace raycore {

struct OrthographicCamera : public Camera
{
	Ray generateRay(const RaySampler::Type &screenPos) const override;
};

}