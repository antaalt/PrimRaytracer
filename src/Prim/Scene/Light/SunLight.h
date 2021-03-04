#pragma once

#include "../../Core/Light.h"

namespace prim {

class SunLight : public Light
{
public:
	SunLight(const norm3f &direction, const color4f &albedo, float intensity);

	Ray sample(const point3f &hitPoint) const override;

	float pdf(const Ray &ray) const override;
private:
	norm3f m_direction;
};

}