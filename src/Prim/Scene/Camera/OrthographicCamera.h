#pragma once

#include "../../Core/Camera.h"

namespace prim {

struct OrthographicCamera : public Camera
{
	Ray generateRay(const RaySampler::Type &screenPos) const override;
};

}