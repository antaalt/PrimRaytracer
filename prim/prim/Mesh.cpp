#include "Mesh.h"

#include "Triangle.h"

namespace prim {

Mesh::Mesh(const mat4f &transform, Material *material) :
	Hitable(transform, material)
{
}

void Mesh::build()
{
	for (const point3f &position : m_positions)
		m_bbox.include(m_transform * position);
}

bool Mesh::intersect(const Ray & ray, Intersection &intersection) const
{
	if (!m_bbox.intersect(ray)) // TODO transform ray with inverse mat
		return false;
	bool terminateOnFirstHit = intersection.terminateOnFirstHit();
	for (const Triangle &tri : m_triangles)
		if (intersectTri(tri, ray, intersection) && terminateOnFirstHit)
			return true;

	return intersection.valid();
}

void Mesh::compute(const point3f &hitPoint, const vec2f & barycentric, Intersection::Indice indice, norm3f * normal, uv2f * texCoord, color4f * color) const
{
	const Triangle &tri = m_triangles[indice];
	*normal = interpolate(m_normals[tri.A], m_normals[tri.B], m_normals[tri.C], barycentric);
	*texCoord = interpolate(m_uvs[tri.A], m_uvs[tri.B], m_uvs[tri.C], barycentric);
	*color = interpolate(m_colors[tri.A], m_colors[tri.B], m_colors[tri.C], barycentric);
}

void Mesh::include(BoundingBox &boundingBox)
{
	boundingBox.include(m_bbox);
}

geometry::point3f Mesh::center(const Mesh::Triangle & tri) const
{
	return (m_transform * m_positions[tri.A] + m_transform * m_positions[tri.B] + m_transform * m_positions[tri.C]) / 3.f;
}

float Mesh::area(const Mesh::Triangle & tri) const
{
	const point3f &A = m_transform * m_positions[tri.A];
	const point3f &B = m_transform * m_positions[tri.B];
	const point3f &C = m_transform * m_positions[tri.C];
	return geometry::vec3f::cross(geometry::vec3f(B - A), geometry::vec3f(C - A)).norm() / 2.f;
}

bool Mesh::intersectTri(const Mesh::Triangle & tri, const Ray & ray, Intersection &intersection) const
{
	const point3f &A = m_transform * m_positions[tri.A];
	const point3f &B = m_transform * m_positions[tri.B];
	const point3f &C = m_transform * m_positions[tri.C];
	// https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
	geometry::vec3f AB(B - A);
	geometry::vec3f AC(C - A);
	geometry::vec3f h = geometry::vec3f::cross(ray.direction, AC);
	float a = geometry::vec3f::dot(AB, h);

	if (intersection.cull(a))
		return false;

	float f = 1.f / a;

	geometry::vec3f s(ray.origin - A);
	float u = f * geometry::vec3f::dot(s, h);
	if (u < 0.f || u > 1.f)
		return false;
	geometry::vec3f q = geometry::vec3f::cross(s, AB);
	float v = f * geometry::vec3f::dot(ray.direction, q);
	if (v < 0.f || u + v > 1.f)
		return false;
	float t = f * geometry::vec3f::dot(AC, q);
	if (t > std::numeric_limits<float>::epsilon() && t > ray.tmin && t < ray.tmax)
	{
		uint32_t iTri = static_cast<uint32_t>(&tri - m_triangles.data());
		return intersection.report(t, vec2f(u, v), this, iTri);
	}
	else
	{
		return false;
	}
}

}
