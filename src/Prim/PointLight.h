#pragma once
#include "Light.h"

namespace prim {

class PointLight : public Light
{
public:
	PointLight(const point3f &center, color4f albedo, float intensity);

	Ray sample(const point3f &hitPoint) const override;
	float pdf(const Ray &ray) const override;
private:
	point3f center;
};

}
