#include "Triangle.h"


namespace raycore {

geometry::norm3f interpolate(const geometry::norm3f &nA, const geometry::norm3f &nB, const geometry::norm3f &nC, vec2f barycentric)
{
	return geometry::norm3f::normalize(geometry::norm3f(nB * barycentric.x + nC * barycentric.y + nA * (1.f - barycentric.x - barycentric.y)));
}

geometry::uv2f interpolate(const geometry::uv2f &tA, const geometry::uv2f &tB, const geometry::uv2f &tC, vec2f barycentric)
{
	return tB * barycentric.x + tC * barycentric.y + tA * (1.f - barycentric.x - barycentric.y);
}

geometry::color4f interpolate(const geometry::color4f &cA, const geometry::color4f &cB, const geometry::color4f &cC, vec2f barycentric)
{
	return cB * barycentric.x + cC * barycentric.y + cA * (1.f - barycentric.x - barycentric.y);
}

bool Triangle::intersect(const Ray & ray, Intersection * intersection) const
{
	// https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
	const float epsilon = 0.0000001f;
	geometry::vec3f AB(B.position - A.position);
	geometry::vec3f AC(C.position - A.position);
	geometry::vec3f h = geometry::vec3f::cross(ray.direction, AC);
	float a = geometry::vec3f::dot(AB, h);

	BackCulling culling;
	if (culling(a))
		return false;

	float f = 1.f / a;
			
	geometry::vec3f s(ray.origin - A.position);
	float u = f * geometry::vec3f::dot(s, h);
	if (u < 0.f || u > 1.f)
		return false;
	geometry::vec3f q = geometry::vec3f::cross(s, AB);
	float v = f * geometry::vec3f::dot(ray.direction, q);
	if (v < 0.f || u + v > 1.f)
		return false;
	float t = f * geometry::vec3f::dot(AC, q);
	if (t > epsilon && t > ray.tmin && t < ray.tmax)
		return intersection->report(t, vec2f(u, v), this);
	else
		return false;
}

geometry::norm3f interpolatenormal(const geometry::norm3f &nA, const geometry::norm3f &nB, const geometry::norm3f &nC, vec2f barycentric)
{
	return geometry::norm3f::normalize(geometry::norm3f(nB * barycentric.x + nC * barycentric.y + nA * (1.f - barycentric.x - barycentric.y)));
}

geometry::uv2f interpolateTexCoord(const geometry::uv2f &tA, const geometry::uv2f &tB, const geometry::uv2f &tC, vec2f barycentric)
{
	return tB * barycentric.x + tC * barycentric.y + tA * (1.f - barycentric.x - barycentric.y);
}

geometry::color4f interpolateColors(const geometry::color4f &cA, const geometry::color4f &cB, const geometry::color4f &cC, vec2f barycentric)
{
	return cB * barycentric.x + cC * barycentric.y + cA * (1.f - barycentric.x - barycentric.y);
}

ComputedIntersection Triangle::compute(const Ray & ray, const Intersection &intersection) const
{
	ComputedIntersection computedIntersection;
	computedIntersection.direction = ray.direction;
	computedIntersection.point = ray(intersection.distance);
	computedIntersection.normal = interpolatenormal(A.normal, B.normal, C.normal, intersection.barycentric);
	computedIntersection.texcoord = interpolateTexCoord(A.texcoord, B.texcoord, C.texcoord, intersection.barycentric);
	computedIntersection.color = interpolateColors(A.color, B.color, C.color, intersection.barycentric);
	computedIntersection.material = m_material;
	return computedIntersection;
}

float Triangle::area() const
{
	return geometry::vec3f::cross(geometry::vec3f(B.position - A.position), geometry::vec3f(C.position - A.position)).norm() / 2.f;
}

}