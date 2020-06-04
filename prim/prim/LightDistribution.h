#pragma once
#include "Light.h"

#include <vector>

namespace prim {

struct Scene;

class LightDistribution
{
public:
	LightDistribution();
	~LightDistribution();

	void addLight(Light* light);

	color4f sampleOneLight(const ComputedIntersection &info, const Scene &scene) const;
private:
	std::vector<Light*> lights;
};
}

