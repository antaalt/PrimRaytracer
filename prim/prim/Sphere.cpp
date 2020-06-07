#include "Sphere.h"
#include "Transform.h"

namespace prim {


Sphere::Sphere(const mat4f & transform, float radius, Material * material) :
	Hitable(transform, material),
	m_radius(radius)
{
}

bool Sphere::intersect(const Ray &worldRay, Intersection *intersection) const
{
	// World to local transform
	const Ray localRay = m_worldToLocal(worldRay);

	vec3f o(localRay.origin);
	vec3f d(localRay.direction);
	
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
	if (t1 > localRay.tmin && t1 < localRay.tmax)
	{
		point3f localHit = localRay(t1);
		localHit *= m_radius / point3f::distance(localHit, point3f(0)); // Refine hit
		point3f worldHit = m_localToWorld(localHit);
		float dist = point3f::distance(worldHit, worldRay.origin);
		return intersection->report(dist, vec2f(0.f), this);
	}
	else if (t2 > localRay.tmin && t2 < localRay.tmax)
	{
		point3f localHit = localRay(t2);
		localHit *= m_radius / point3f::distance(localHit, point3f(0)); // Refine hit
		point3f worldHit = m_localToWorld(localHit);
		float dist = point3f::distance(worldHit, worldRay.origin);
		return intersection->report(dist, vec2f(0.f), this);
	}
	else
		return false;
}

void Sphere::compute(const point3f &worldHit, const vec2f & barycentric, Intersection::Indice indice, norm3f * normal, uv2f * texCoord) const
{
	// Local to world transform
	const point3f center(0);
	const point3f localHit = m_worldToLocal(worldHit);
	const norm3f localNormal = norm3f::normalize(norm3f(localHit - center));
	*normal = m_localToWorld(localNormal);
	// https://en.wikipedia.org/wiki/UV_mapping
	const float u = 0.5f + geometry::arctan2(-localNormal.z, -localNormal.x) / (2.f * geometry::pi<float>);
	const float v = 0.5f - geometry::arcsin(-localNormal.y)() / geometry::pi<float>();
	*texCoord = uv2f(u, v);
}

float Sphere::area() const
{
	return 4.f * geometry::pi<float>() * m_radius * m_radius;
}

void Sphere::include(BoundingBox &boundingBox)
{
	// Local to world transform
	boundingBox.include(m_localToWorld(point3f(0) + point3f(m_radius)));
	boundingBox.include(m_localToWorld(point3f(0) - point3f(m_radius)));
}

}