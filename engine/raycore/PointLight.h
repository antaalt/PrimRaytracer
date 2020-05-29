#pragma once
#include "Light.h"

namespace raycore {

class PointLight : public Light
{
public:
	PointLight(const point3f &center, color4f albedo, float intensity);
	virtual bool sample(const ComputedIntersection &info, const Scene &scene, float *pdf, vec3f *sample) const;
	virtual float contribution(const ComputedIntersection &info) const;
private:
	point3f center;
};

}
