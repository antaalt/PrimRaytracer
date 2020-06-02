#pragma once

#include "Light.h"

namespace prim {

class SunLight : Light
{
public:
	SunLight(const norm3f &direction, const color4f &albedo, float intensity);

	// Sample the light from the given intersection, and check if it's occluded.
	bool sample(const ComputedIntersection &info, const Scene &scene, float *pdf, vec3f *sample) const override;
	// Get contribution of light 
	float contribution(const ComputedIntersection &info) const override;
private:
	norm3f m_direction;
};

}