#include "Triangle.h"


namespace prim {

norm3f Triangle::interpolate(const norm3f &nA, const norm3f &nB, const norm3f &nC, vec2f barycentric)
{
	return norm3f::normalize(norm3f(nB * barycentric.x + nC * barycentric.y + nA * (1.f - barycentric.x - barycentric.y)));
}

uv2f Triangle::interpolate(const uv2f &tA, const uv2f &tB, const uv2f &tC, vec2f barycentric)
{
	return tB * barycentric.x + tC * barycentric.y + tA * (1.f - barycentric.x - barycentric.y);
}

Triangle::Triangle(Material * material) :
	Hitable(material)
{
}

Triangle::Triangle(const Vertex & A, const Vertex & B, const Vertex & Cs, Material * material) :
	Hitable(material),
	A(A), B(B), C(C)
{
}

bool Triangle::intersect(const Ray & ray, Intersection *intersection) const
{
	// https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
	// This algorithm require ray direction to be normalized. So we normalize
	// the direction and keep the normalization factor to rescale final t.
	// This let us support intersection which where scaled by a scaled transform.
	const float normalizationScale = ray.direction.norm();
	const point3f o = ray.origin;
	const vec3f d = ray.direction / normalizationScale;
	
	vec3f AB(B.position - A.position);
	vec3f AC(C.position - A.position);
	vec3f h = vec3f::cross(d, AC);
	float det = vec3f::dot(AB, h);

	if (ray.cull(det))
		return false;

	float invDet = 1.f / det;
			
	vec3f s(o - A.position);
	float u = invDet * vec3f::dot(s, h);
	if (u < 0.f || u > 1.f)
		return false;
	vec3f q = vec3f::cross(s, AB);
	float v = invDet * vec3f::dot(d, q);
	if (v < 0.f || u + v > 1.f)
		return false;
	float t = invDet * vec3f::dot(AC, q);
	if (t > std::numeric_limits<float>::epsilon() && t > ray.tmin && t < ray.tmax)
	{
		const vec2f barycentric = vec2f(u, v);
		const point3f hitPoint = ray(t / normalizationScale);
		const norm3f normal = interpolate(A.normal, B.normal, C.normal, barycentric);
		const uv2f texCoord = interpolate(A.texcoord, B.texcoord, C.texcoord, barycentric);
		intersection->report(hitPoint, normal, texCoord, m_material);
		ray.tmax = t; // update closest hit
		return true;
	}
	else
		return false;
}

void Triangle::include(BoundingBox & boundingBox) const
{
	boundingBox.include(A.position);
	boundingBox.include(B.position);
	boundingBox.include(C.position);
}

float Triangle::area() const
{
	return vec3f::cross(vec3f(B.position - A.position), vec3f(C.position - A.position)).norm() / 2.f;
}

point3f Triangle::center() const
{
	return (A.position + B.position + C.position) / 3.f;
}

}