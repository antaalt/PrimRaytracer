#pragma once
#include "Ray.h"
#include "Scene.h"

namespace prim {

class Tracer {
public:
	virtual geometry::color4f render(const Ray &ray, const Scene &scene) const = 0;
};

}


