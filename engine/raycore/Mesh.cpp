#include "Mesh.h"

#include "Triangle.h"

namespace raycore {

Mesh::Mesh(const mat4f &transform, Material *material) :
	Hitable(transform, material)
{
}

void Mesh::build()
{
	for (const point3f &position : m_positions)
		m_bbox.include(m_transform * position);
}

bool Mesh::intersect(const Ray & ray, Intersection * intersection) const
{
	if (!m_bbox.intersect(ray)) // TODO transform ray with inverse mat
		return false;
	for (uint32_t iTri = 0; iTri < m_triangles.size(); iTri ++)
		intersectTri(m_triangles[iTri], ray, intersection);

	return intersection->valid();
}

ComputedIntersection Mesh::compute(const Ray & ray, const Intersection & intersection) const
{
	const Triangle *tri = static_cast<const Triangle*>(intersection.data);
	ComputedIntersection computedIntersection;
	computedIntersection.direction = ray.direction;
	computedIntersection.point = ray(intersection.distance);
	computedIntersection.normal = interpolate(m_normals[tri->A], m_normals[tri->B], m_normals[tri->C], intersection.barycentric);
	computedIntersection.texcoord = interpolate(m_uvs[tri->A], m_uvs[tri->B], m_uvs[tri->C], intersection.barycentric);
	computedIntersection.color = interpolate(m_colors[tri->A], m_colors[tri->B], m_colors[tri->C], intersection.barycentric);
	computedIntersection.material = m_material;
	return computedIntersection;
}

geometry::point3f Mesh::center(const Mesh::Triangle & tri)
{
	return (m_transform * m_positions[tri.A] + m_transform * m_positions[tri.B] + m_transform * m_positions[tri.C]) / 3.f;
}

float Mesh::area(const Mesh::Triangle & tri)
{
	const point3f &A = m_transform * m_positions[tri.A];
	const point3f &B = m_transform * m_positions[tri.B];
	const point3f &C = m_transform * m_positions[tri.C];
	return geometry::vec3f::cross(geometry::vec3f(B - A), geometry::vec3f(C - A)).norm() / 2.f;
}

bool Mesh::intersectTri(const Mesh::Triangle & tri, const Ray & ray, Intersection * intersection) const
{
	const point3f &A = m_transform * m_positions[tri.A];
	const point3f &B = m_transform * m_positions[tri.B];
	const point3f &C = m_transform * m_positions[tri.C];
	// https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
	geometry::vec3f AB(B - A);
	geometry::vec3f AC(C - A);
	geometry::vec3f h = geometry::vec3f::cross(ray.direction, AC);
	float a = geometry::vec3f::dot(AB, h);

	BackCulling backCulling; // TODO pass as tempalte ?
	if (backCulling(a))
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
		return intersection->report(t, vec2f(u, v), this, &tri);
	}
	else
	{
		return false;
	}
}

}
