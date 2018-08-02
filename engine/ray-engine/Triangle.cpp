#include "Triangle.h"


namespace app {
	namespace prim {


		Vertex::Vertex()
		{
		}
		Vertex::Vertex(Point3 p, Normal n, Texcoord tc, ColorHDR c) : position(p), normal(n), texcoord(tc), color(c)
		{
		}

		Triangle::Triangle()
		{
		}

		Triangle::Triangle(const Vertex & vA, const Vertex & vB, const Vertex & vC) :
			A(vA), B(vB), C(vC)
		{
		}


		Triangle::~Triangle()
		{
		}
		bool Triangle::intersect(const tracer::Ray & ray, Intersection & intersection) const
		{
			const Vector3 e0 = B.position - A.position;
			const Vector3 e1 = A.position - C.position;
			const Normal n = Vector3::cross(e1, e0);
			const Vector3 e2 = (1.0f / Vector3::dot(n, ray.direction)) * (A.position - ray.origin);
			const Vector3 i = Vector3::cross(ray.direction, e2);
			float beta = Vector3::dot(i, e1);
			float gamma = Vector3::dot(i, e0);
			float t = Vector3::dot(n, e2);
			const bool hit = ((t<ray.tmax) & (t>ray.tmin) & (beta >= 0.0f) & (gamma >= 0.0f) & (beta + gamma <= 1));
			if (hit)
			{
				intersection.distance = t;
				intersection.beta = beta;
				intersection.gamma = gamma;
				intersection.hitable = this;
				return true;
			}
			return false;
		}
		HitInfo Triangle::computeIntersection(const tracer::Ray & ray, const Intersection & intersection) const
		{
			HitInfo info;
			info.point = ray.origin + ray.direction * intersection.distance;
			info.normal = Normal::normalize(B.normal * intersection.beta + C.normal * intersection.gamma + A.normal * (1.f - intersection.beta - intersection.gamma));
			info.texcoord = B.texcoord * intersection.beta + C.texcoord * intersection.gamma + A.texcoord * (1.f - intersection.beta - intersection.gamma);
			info.color = B.color * intersection.beta + C.color * intersection.gamma + A.color * (1.f - intersection.beta - intersection.gamma);
			info.material = material;
			return info;
		}
	}
}