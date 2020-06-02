#include "Sphere.h"
#include "Transform.h"

namespace prim {


Sphere::Sphere(const mat4f & transform, float radius, Material * material) :
	Hitable(transform, material),
	m_radius(radius)
{
}

bool Sphere::intersect(const Ray &ray, Intersection &intersection) const
{
	vec3f eyeDirection(point3f(m_transform[3]) - ray.origin);
	float b = vec3f::dot(eyeDirection, ray.direction); // a is always 1 because dot of same normalized vector
	float det = b * b - vec3f::dot(eyeDirection, eyeDirection) + m_radius * m_radius;
	if (det < 0)
		return false;
	else
		det = sqrt(det);

	float t1 = b - det;
	float t2 = b + det;
	if (t1 > ray.tmin && t1 < ray.tmax)
		return intersection.report(t1, vec2f(0.f), this);
	else if (t2 > ray.tmin && t2 < ray.tmax)
		return intersection.report(t2, vec2f(0.f), this);
	else
		return false;
}

void Sphere::compute(const point3f &hitPoint, const vec2f & barycentric, Intersection::Indice indice, norm3f * normal, uv2f * texCoord, color4f * color) const
{
	*normal = norm3f::normalize(norm3f(hitPoint - point3f(m_transform[3])));
	// https://en.wikipedia.org/wiki/UV_mapping
	const norm3f up = norm3f(m_transform[1]);
	transform::Onb onb(up);
	const norm3f n = onb(*normal);
	const float u = 0.5f + geometry::arctan2(-n.z, -n.x) / (2.f * geometry::pi<float>);
	const float v = 0.5f - geometry::arcsin(-n.y)() / geometry::pi<float>();
	*texCoord = uv2f(u, v);
	*color = color4f(1.f);
}

float Sphere::area() const
{
	return 4.f * geometry::pi<float>() * m_radius * m_radius;
}

void Sphere::include(BoundingBox &boundingBox)
{
	point3f center = point3f(m_transform[3]);
	boundingBox.include(center + point3f(m_radius));
	boundingBox.include(center - point3f(m_radius));
}

}