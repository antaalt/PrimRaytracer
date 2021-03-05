#pragma once
#include "../../Core/Geometry.h"
#include "../../Core/Light.h"
#include "../../Core/Shape.h"

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
