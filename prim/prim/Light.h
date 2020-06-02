#pragma once
#include "Config.h"
#include "Ray.h"
#include "Hitable.h"
#include "Scene.h"

namespace prim {

struct Scene;
		
class Light
{
public:
	Light(const color4f &albedo, float intensity);

	// Sample the light from the given intersection, and check if it's occluded.
	virtual bool sample(const ComputedIntersection &info, const Scene &scene, float *pdf, vec3f *sample) const = 0;
	// Get contribution of light 
	virtual float contribution(const ComputedIntersection &info) const = 0;
	// Get color of light
	virtual color4f shade() const { return m_albedo * m_intensity; }
protected:
	float m_intensity; // Intensity of the light
	color4f m_albedo; // Albedo of the light
};

}

