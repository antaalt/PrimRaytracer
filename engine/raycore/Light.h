#pragma once
#include "Config.h"
#include "Ray.h"
#include "Hitable.h"
#include "Scene.h"

namespace raycore {

struct Scene;
		
class Light
{
public:
	Light(color4f albedo, float intensity);
	virtual bool sample(const ComputedIntersection &info, const Scene &scene, float *pdf, vec3f *sample) const = 0;
	virtual float contribution(const ComputedIntersection &info) const = 0;
	virtual color4f shade() const
	{
		return albedo * intensity;
	}
protected:
	float intensity;
	color4f albedo;
};

}

