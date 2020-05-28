#pragma once
#include "Light.h"
namespace raycore {
namespace prim {

struct Scene;

class LightDistribution
{
public:
	LightDistribution();
	~LightDistribution();

	void addLight(Light* light);

	color4f sampleOneLight(const HitInfo &info, const Scene &scene) const;
private:
	std::vector<Light*> lights;
};
}
}

