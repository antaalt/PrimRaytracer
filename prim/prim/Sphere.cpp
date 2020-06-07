#include "Sphere.h"
#include "Transform.h"

namespace prim {


Sphere::Sphere(float radius, Material * material) :
	Hitable(material),
	m_radius(radius)
{
}

bool Sphere::intersect(const Ray &ray, Intersection *intersection) const
{
	// World to local transform

	vec3f o(ray.origin);
	vec3f d(ray.direction);
	
	float a = vec3f::dot(d, d);
	float b = 2.f * vec3f::dot(d, o);
	float c = vec3f::dot(o, o) - m_radius * m_radius;
	float discriminant = b * b - 4.f * a * c;

	if (discriminant < 0)
		return false;
	
	float delta = sqrt(discriminant);

	float t1 = (-b - delta) / (2.f * a);
	float t2 = (-b + delta) / (2.f * a);
	// Local to world transform
	if (t1 > ray.tmin && t1 < ray.tmax)
	{
		// Local to world transform
		const point3f center(0);
		point3f hitPoint = ray(t1);
		hitPoint *= m_radius / point3f::distance(hitPoint, center); // Refine hit
		const norm3f normal = norm3f::normalize(norm3f(hitPoint - center));
		// https://en.wikipedia.org/wiki/UV_mapping
		const float u = 0.5f + geometry::arctan2(-normal.z, -normal.x) / (2.f * geometry::pi<float>);
		const float v = 0.5f - geometry::arcsin(-normal.y)() / geometry::pi<float>();
		const uv2f texcoord = uv2f(u, v);
		intersection->report(hitPoint, normal, texcoord, m_material);
		ray.tmax = t1;
		return true;

	}
	else if (t2 > ray.tmin && t2 < ray.tmax)
	{
		// Local to world transform
		const point3f center(0);
		point3f hitPoint = ray(t2);
		hitPoint *= m_radius / point3f::distance(hitPoint, center); // Refine hit
		const norm3f normal = norm3f::normalize(norm3f(hitPoint - center));
		// https://en.wikipedia.org/wiki/UV_mapping
		const float u = 0.5f + geometry::arctan2(-normal.z, -normal.x) / (2.f * geometry::pi<float>);
		const float v = 0.5f - geometry::arcsin(-normal.y)() / geometry::pi<float>();
		const uv2f texcoord = uv2f(u, v);
		intersection->report(hitPoint, normal, texcoord, m_material);
		ray.tmax = t2;
		return true;
	}
	else
		return false;
}

float Sphere::area() const
{
	return 4.f * geometry::pi<float>() * m_radius * m_radius;
}

void Sphere::include(BoundingBox &boundingBox) const
{
	// Local to world transform
	boundingBox.include(point3f(0) + point3f(m_radius));
	boundingBox.include(point3f(0) - point3f(m_radius));
}

}