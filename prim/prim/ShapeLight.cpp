#include "ShapeLight.h"



namespace prim {

ShapeLight::ShapeLight(Shape * shape, color4f albedo, float intensity) : Light(albedo, intensity), m_shape(shape)
{
}
ShapeLight::~ShapeLight()
{
}
bool ShapeLight::sample(const ComputedIntersection & info, const Scene & scene, float * pdf, vec3f * sample) const
{
	vec3f s = m_shape->sample(info.point);
	ComputedIntersection info2;
	if (scene.intersect(Ray(info.point, vec3f::normalize(s), EPSILON, sample->norm()), &info2))
		return false;
	*pdf = m_shape->pdf();
	*sample = s;
	return true;
}
float ShapeLight::contribution(const ComputedIntersection & info) const
{
	float d = point3f::distance(info.point, m_shape->position());
	return m_intensity / (d * d);
}

}


