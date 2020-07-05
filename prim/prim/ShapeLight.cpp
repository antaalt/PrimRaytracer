#include "ShapeLight.h"

#include "Scene.h"

namespace prim {

ShapeLight::ShapeLight(Shape * shape, color4f albedo, float intensity) : Light(albedo, intensity), m_shape(shape)
{
}
Ray ShapeLight::sample(const point3f & hitPoint) const
{
	Ray ray;
	ray.origin = hitPoint;
	ray.direction = m_shape->sample(hitPoint);
	ray.tmax = ray.direction.norm();
	ray.direction /= ray.tmax;
	return ray;
}

float ShapeLight::pdf(const Ray & ray) const
{
	return m_shape->pdf();
}

/*float ShapeLight::contribution(const ComputedIntersection & info) const
{
	float d = point3f::distance(info.point, m_shape->position());
	return m_intensity / (d * d);
}*/

}


