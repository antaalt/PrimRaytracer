#pragma once
#include "Geometry.h"
#include "Ray.h"
#include "Hitable.h"

namespace prim {

struct Scene;
		
class Light
{
public:
	Light(const color4f &albedo, float intensity);

	virtual Ray sample(const point3f &hitPoint) const = 0;
	virtual float pdf(const Ray &ray) const = 0;
	color4f emission() const { return m_albedo * m_intensity; }
protected:
	float m_intensity; // Intensity of the light
	color4f m_albedo; // Albedo of the light
};

}

