#include "ShapeLight.h"



namespace raycore {
namespace prim {
ShapeLight::ShapeLight(shape::Shape * shape, color4f albedo, float intensity) : Light(albedo, intensity), shape(shape)
{
}
ShapeLight::~ShapeLight()
{
	delete shape;
}
bool ShapeLight::sample(const prim::HitInfo & info, const prim::Scene & scene, float * pdf, vec3f * sample) const
{
	vec3f s = shape->sample(info.point);
	if (scene.isOccluded(tracer::Ray(info.point, vec3f::normalize(s), tracer::GEOMETRY_RAY, EPSILON, sample->norm())))
		return false;
	*pdf = shape->pdf();
	*sample = s;
	return true;
}
float ShapeLight::contribution(const prim::HitInfo & info) const
{
	float d = point3f::distance(info.point, shape->position());
	return intensity / (d * d);
}

namespace shape {

Sphere::Sphere(const point3f & center, float radius) : center(center), radius(radius)
{
}
vec3f Sphere::sample(const point3f & from) const
{
	vec3f sample = sample::unitSphere(rand::rnd(), rand::rnd());
	return vec3f(center - from) + sample * radius;
}
float Sphere::pdf() const
{
	return 4.f * geometry::pi<float>() * radius * radius;;
}
point3f Sphere::position() const
{
	return center;
}
Disk::Disk(const point3f & center, float radius, const norm3f &up) : center(center), radius(radius), up(up)
{
}
vec3f Disk::sample(const point3f & from) const
{
	vec3f sample = vec3f(sample::unitDisk(rand::rnd(), rand::rnd()), 1.f);
	transform::Onb onb(up);
	return vec3f(center) + onb(sample * radius);
}
float Disk::pdf() const
{
	return geometry::pi<float>() * radius * radius;
}
point3f Disk::position() const
{
	return center;
}
}
}
}


