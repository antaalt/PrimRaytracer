#include "Triangle.h"


namespace raycore {
namespace prim {


Vertex::Vertex()
{
}
Vertex::Vertex(geometry::point3f p, geometry::norm3f n, geometry::uv2f tc, geometry::color4f c) : position(p), normal(n), texcoord(tc), color(c)
{
}

Triangle::Triangle()
{
}
Triangle::Triangle(const Vertex & vA, const Vertex & vB, const Vertex & vC) :
	A(vA), B(vB), C(vC)
{
}
bool Triangle::intersect(const tracer::Ray & ray, Intersection * intersection) const
{
#if defined(MOLLER_TRUMBORE)
	// https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
	const float epsilon = 0.0000001f;
	geometry::vec3f AB(B.position - A.position);
	geometry::vec3f AC(C.position - A.position);
	geometry::vec3f h = geometry::vec3f::cross(ray.direction, AC);
	float a = geometry::vec3f::dot(AB, h);
#if defined(BACKFACE_CULLING)
	if (a < epsilon) 
		return false;
#else 
	if (fabs(a) < epsilon)
		return false;
#endif 
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
		return intersection->set(t, this, u, v);
	else
		return false;
#else 
#error "not working"
	const geometry::vec3f AB = B.position - A.position;
	const geometry::vec3f AC = C.position - A.position;
	const normal geonormal = geometry::vec3f::cross(AB, AC);
	float denom = geometry::vec3f::dot(geonormal, geonormal);

	float NdotRD = geometry::vec3f::dot(geonormal, ray.direction);
#if defined(BACKFACE_CULLING)
#else
	if (fabs(NdotRD) < EPSILON)
		return false;
#endif

	float d = geometry::vec3f::dot(geonormal, A.position);

	float t = (geometry::vec3f::dot(geonormal, ray.origin) + d) / NdotRD;
	if (t < 0)
		return false;

	float u, v;
	geometry::vec3f P = ray.origin + t * ray.direction;

	geometry::vec3f C2;

	geometry::vec3f AP = P - A.position;
	C2 = geometry::vec3f::cross(AB, AP);
	if (geometry::vec3f::dot(geonormal, C2) < 0)
		return false;

	geometry::vec3f BC = C.position - B.position;
	geometry::vec3f BP = P - B.position;
	C2 = geometry::vec3f::cross(BC, BP);
	if ((u = geometry::vec3f::dot(geonormal, C2)) < 0)
		return false;

	geometry::vec3f CA = A.position - C.position;
	geometry::vec3f CP = P - C.position;
	C2 = geometry::vec3f::cross(CA, CP);
	if ((v = geometry::vec3f::dot(geonormal, C2)) < 0)
		return false;

	u /= denom;
	v /= denom;

	intersection.beta = u;
	intersection.gamma = v;
	intersection.distance = t;
	intersection.hitable = this;

	return true;
#endif
}

geometry::norm3f interpolatenormal(const geometry::norm3f &nA, const geometry::norm3f &nB, const geometry::norm3f &nC, float alpha, float beta)
{
	return geometry::norm3f::normalize(geometry::norm3f(nB * alpha + nC * beta + nA * (1.f - alpha - beta)));
}
geometry::uv2f interpolateTexCoord(const geometry::uv2f &tA, const geometry::uv2f &tB, const geometry::uv2f &tC, float alpha, float beta)
{
	return tB * alpha + tC * beta + tA * (1.f - alpha - beta);
}
geometry::color4f interpolateColors(const geometry::color4f &cA, const geometry::color4f &cB, const geometry::color4f &cC, float alpha, float beta)
{
	return cB * alpha + cC * beta + cA * (1.f - alpha - beta);
}

HitInfo Triangle::computeIntersection(const tracer::Ray & ray, const Intersection * intersection) const
{
	HitInfo info;
	info.direction = ray.direction;
	info.point = intersection->computeHit(ray);
	float alpha = intersection->getAlpha();
	float beta = intersection->getBeta();
	info.normal = interpolatenormal(A.normal, B.normal, C.normal, alpha, beta);
	info.texcoord = interpolateTexCoord(A.texcoord, B.texcoord, C.texcoord, alpha, beta);
	info.color = interpolateColors(A.color, B.color, C.color, alpha, beta);
	info.material = material;
	return info;
}
BoundingBox Triangle::computeBoundingBox() const
{
	BoundingBox bbox;
	bbox.include(A.position);
	bbox.include(B.position);
	bbox.include(C.position);
	return bbox;
}
float Triangle::area() const
{
	return geometry::vec3f::cross(geometry::vec3f(B.position - A.position), geometry::vec3f(C.position - A.position)).norm() / 2.f;
}
}
}