#include "Triangle.h"


namespace prim {

norm3f interpolate(const norm3f &nA, const norm3f &nB, const norm3f &nC, vec2f barycentric)
{
	return norm3f::normalize(norm3f(nB * barycentric.x + nC * barycentric.y + nA * (1.f - barycentric.x - barycentric.y)));
}

uv2f interpolate(const uv2f &tA, const uv2f &tB, const uv2f &tC, vec2f barycentric)
{
	return tB * barycentric.x + tC * barycentric.y + tA * (1.f - barycentric.x - barycentric.y);
}

color4f interpolate(const color4f &cA, const color4f &cB, const color4f &cC, vec2f barycentric)
{
	return cB * barycentric.x + cC * barycentric.y + cA * (1.f - barycentric.x - barycentric.y);
}

bool Triangle::intersect(const Ray & worldRay, Intersection &intersection) const
{
	// https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
	// This algorithm require ray direction to be normalized. So we normalize
	// the direction and keep the normalization factor to rescale final t.
	const Ray localRay = m_worldToLocal(worldRay);
	const float normalizationScale = localRay.direction.norm();
	const point3f o = localRay.origin;
	const vec3f d = localRay.direction / normalizationScale;

	vec3f AB(B.position - A.position);
	vec3f AC(C.position - A.position);
	vec3f h = vec3f::cross(d, AC);
	float a = vec3f::dot(AB, h);

	if (intersection.cull(a))
		return false;

	float f = 1.f / a;
			
	vec3f s(o - A.position);
	float u = f * vec3f::dot(s, h);
	if (u < 0.f || u > 1.f)
		return false;
	vec3f q = vec3f::cross(s, AB);
	float v = f * vec3f::dot(d, q);
	if (v < 0.f || u + v > 1.f)
		return false;
	float t = f * vec3f::dot(AC, q);
	if (t > std::numeric_limits<float>::epsilon() && t > localRay.tmin && t < localRay.tmax)
		return intersection.report(t * normalizationScale, vec2f(u, v), this);
	else
		return true;
}

void Triangle::compute(const point3f & hitPoint, const vec2f & barycentric, Intersection::Indice indice, norm3f * normal, uv2f * texCoord, color4f * color) const
{
	*normal = norm3f::normalize(m_localToWorld(interpolate(A.normal, B.normal, C.normal, barycentric)));
	*texCoord = interpolate(A.texcoord, B.texcoord, C.texcoord, barycentric);
	*color = interpolate(A.color, B.color, C.color, barycentric);
}

float Triangle::area() const
{
	return geometry::vec3f::cross(geometry::vec3f(B.position - A.position), geometry::vec3f(C.position - A.position)).norm() / 2.f;
}

}