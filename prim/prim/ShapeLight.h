#pragma once
#include "Config.h"
#include "Random.h"
#include "Light.h"
#include "Shape.h"

namespace prim {

class ShapeLight : public Light
{
public:
	ShapeLight(Shape *shape, color4f albedo, float intensity);
	~ShapeLight();
	virtual bool sample(const ComputedIntersection &info, const Scene &scene, float *pdf, vec3f *sample) const;
	float contribution(const ComputedIntersection &info) const;
private:
	Shape *m_shape;
};

}
