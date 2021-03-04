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

	Ray sample(const point3f &hitPoint) const override;
	float pdf(const Ray &ray) const override;
private:
	Shape *m_shape;
};

}
