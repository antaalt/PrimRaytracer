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
		m_bbox.include(position);
}

bool Mesh::intersect(const Ray & worldRay, Intersection *intersection) const
{
	// World to local transform
	Ray localRay = m_worldToLocal(worldRay);
	if (!m_bbox.intersect(localRay))
		return false;
	bool terminateOnFirstHit = intersection->terminateOnFirstHit();
	Intersection localIntersection(terminateOnFirstHit);
	for (const Triangle &tri : m_triangles)
	{
		if (intersectTri(tri, localRay, localIntersection) && terminateOnFirstHit)
		{
			// local Intersection to world intersection
			point3f localHit = localRay(localIntersection.getDistance());
			point3f worldHit = m_localToWorld(localHit);
			return intersection->report(point3f::distance(worldHit, worldRay.origin), localIntersection.getBarycentric(), this, localIntersection.getIndice());
		}
	}
	if (localIntersection.valid())
	{
		// local Intersection to world intersection
		point3f localHit = localRay(localIntersection.getDistance());
		point3f worldHit = m_localToWorld(localHit);
		return intersection->report(point3f::distance(worldHit, worldRay.origin), localIntersection.getBarycentric(), this, localIntersection.getIndice());
	}
	else
	{
		return false;
	}
}

void Mesh::compute(const point3f &worldHit, const vec2f & barycentric, Intersection::Indice indice, norm3f * normal, uv2f * texCoord) const
{
	const Triangle &tri = m_triangles[indice];
	*normal = norm3f::normalize(m_localToWorld(interpolate(m_normals[tri.A], m_normals[tri.B], m_normals[tri.C], barycentric)));
	*texCoord = interpolate(m_uvs[tri.A], m_uvs[tri.B], m_uvs[tri.C], barycentric);
}

void Mesh::include(BoundingBox &boundingBox)
{
	boundingBox.include(m_localToWorld(m_bbox));
}

point3f Mesh::center(const Mesh::Triangle & tri) const
{
	return (m_positions[tri.A] + m_positions[tri.B] + m_positions[tri.C]) / 3.f;
}

float Mesh::area(const Mesh::Triangle & tri) const
{
	const point3f &A = m_positions[tri.A];
	const point3f &B = m_positions[tri.B];
	const point3f &C = m_positions[tri.C];
	return geometry::vec3f::cross(geometry::vec3f(B - A), geometry::vec3f(C - A)).norm() / 2.f;
}

bool Mesh::intersectTri(const Mesh::Triangle & tri, const Ray & localRay, Intersection &localIntersection) const
{
	const point3f &A = m_positions[tri.A];
	const point3f &B = m_positions[tri.B];
	const point3f &C = m_positions[tri.C];
	// https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
	// This algorithm require ray direction to be normalized. So we normalize
	// the direction and keep the normalization factor to rescale final t.
	const float normalizationScale = localRay.direction.norm();
	const point3f o = localRay.origin;
	const vec3f d = localRay.direction / normalizationScale;

	const vec3f AB(B - A);
	const vec3f AC(C - A);
	const vec3f h = vec3f::cross(d, AC);
	const float det = vec3f::dot(AB, h);

	if (localRay.cull(det))
		return false;

	float invDet = 1.f / det;

	const vec3f s(o - A);
	const float u = invDet * vec3f::dot(s, h);
	if (u < 0.f || u > 1.f)
		return false;
	const vec3f q = vec3f::cross(s, AB);
	const float v = invDet * vec3f::dot(d, q);
	if (v < 0.f || u + v > 1.f)
		return false;
	const float t = invDet * vec3f::dot(AC, q);
	// TODO scale tmin & tmax
	if (t > std::numeric_limits<float>::epsilon() && t > localRay.tmin && t < localRay.tmax)
	{
		uint32_t iTri = static_cast<uint32_t>(&tri - m_triangles.data());
		return localIntersection.report(t * normalizationScale, vec2f(u, v), this, iTri);
	}
	else
	{
		return false;
	}
}

}
