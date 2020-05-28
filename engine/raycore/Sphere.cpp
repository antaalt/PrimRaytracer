#include "Sphere.h"
#include "transform.h"

namespace raycore {
namespace prim {

Sphere::Sphere()
{
}

Sphere::Sphere(const point3f & center, float radius, const norm3f up) : center(center), radius(radius), up(up)
{
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const tracer::Ray & ray, Intersection * intersection) const
{
	vec3f eyeDirection(this->center - ray.origin);
	float b = vec3f::dot(eyeDirection, ray.direction); // a is always 1 because dot of same normalized vector
	float det = b * b - vec3f::dot(eyeDirection, eyeDirection) + this->radius * this->radius;
	if (det < 0)
		return false;
	else
		det = sqrt(det);

	float t1 = b - det;
	float t2 = b + det;
	if (t1 > ray.tmin && t1 < ray.tmax)
		return intersection->set(t1, this);
	else if (t2 > ray.tmin && t2 < ray.tmax)
		return intersection->set(t2, this);
	else
		return false;
}

HitInfo Sphere::computeIntersection(const tracer::Ray & ray, const Intersection * intersection) const
{
	HitInfo info;
	info.direction = ray.direction;
	info.point = intersection->computeHit(ray);
	info.normal = norm3f::normalize(norm3f(info.point - this->center));
	// https://en.wikipedia.org/wiki/UV_mraycoreing
	transform::Onb onb(this->up);
	norm3f n = onb(info.normal);
	float u = 0.5f + geometry::arctan2(-n.z, -n.x)() / (2.f * geometry::pi<float>());
	float v = 0.5f - geometry::arcsin(-n.y)() / geometry::pi<float>();
	info.texcoord = uv2f(u, v);
	info.color = color4f(1.f);
	info.material = material;
	return info;
}

BoundingBox Sphere::computeBoundingBox() const
{
	BoundingBox bbox;
	bbox.min = this->center - point3f(this->radius);
	bbox.max = this->center + point3f(this->radius);
	return bbox;
}

float Sphere::area() const
{
	return 4.f * geometry::pi<float>() * this->radius * this->radius;
}

}
}