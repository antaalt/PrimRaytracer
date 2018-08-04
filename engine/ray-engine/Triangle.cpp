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
#if defined(MOLLER_TRUMBORE)
			// https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
			const float epsilon = 0.0000001f;
			Vector3 AB = B.position - A.position;
			Vector3 AC = C.position - A.position;
			Vector3 h = Vector3::cross(ray.direction, AC);
			float a = Vector3::dot(AB, h);
#if defined(BACKFACE_CULLING)
			if (a < epsilon) 
				return false;
#else 
			if (fabs(a) < epsilon)
				return false;
#endif 
			float f = 1.f / a;
			
			Vector3 s = ray.origin - A.position;
			float u = f * (Vector3::dot(s, h));
			if (u < 0.f || u > 1.f)
				return false;
			Vector3 q = Vector3::cross(s, AB);
			float v = f * Vector3::dot(ray.direction, q);
			if (v < 0.f || u + v > 1.f)
				return false;
			float t = f * Vector3::dot(AC, q);
			if (t > epsilon && t > ray.tmin && t < ray.tmax)
				return intersection.set(t, this, u, v);
			else
				return false;
#else 
#error "not working"
			const Vector3 AB = B.position - A.position;
			const Vector3 AC = C.position - A.position;
			const Normal geoNormal = Vector3::cross(AB, AC);
			float denom = Vector3::dot(geoNormal, geoNormal);

			float NdotRD = Vector3::dot(geoNormal, ray.direction);
#if defined(BACKFACE_CULLING)
#else
			if (fabs(NdotRD) < EPSILON)
				return false;
#endif

			float d = Vector3::dot(geoNormal, A.position);

			float t = (Vector3::dot(geoNormal, ray.origin) + d) / NdotRD;
			if (t < 0)
				return false;

			float u, v;
			Vector3 P = ray.origin + t * ray.direction;

			Vector3 C2;

			Vector3 AP = P - A.position;
			C2 = Vector3::cross(AB, AP);
			if (Vector3::dot(geoNormal, C2) < 0)
				return false;

			Vector3 BC = C.position - B.position;
			Vector3 BP = P - B.position;
			C2 = Vector3::cross(BC, BP);
			if ((u = Vector3::dot(geoNormal, C2)) < 0)
				return false;

			Vector3 CA = A.position - C.position;
			Vector3 CP = P - C.position;
			C2 = Vector3::cross(CA, CP);
			if ((v = Vector3::dot(geoNormal, C2)) < 0)
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

		inline Normal interpolateNormal(const Normal &nA, const Normal &nB, const Normal &nC, float u, float v)
		{
			return Normal::normalize(nB * u + nC * v + nA * (1.f - u - v));
		}
		inline Texcoord interpolateTexCoord(const Texcoord &tA, const Texcoord &tB, const Texcoord &tC, float u, float v)
		{
			return tB * u + tC * v + tA * (1.f - u - v);
		}
		inline ColorHDR interpolateColors(const ColorHDR &cA, const ColorHDR &cB, const ColorHDR &cC, float u, float v)
		{
			return cB * u + cC * v + cA * (1.f - u - v);
		}

		HitInfo Triangle::computeIntersection(const tracer::Ray & ray, const Intersection & intersection) const
		{
			HitInfo info;
			info.point = ray.origin + ray.direction * intersection.getDistance();
			info.normal = interpolateNormal(A.normal, B.normal, C.normal, intersection.getU(), intersection.getV());
			info.texcoord = interpolateTexCoord(A.texcoord, B.texcoord, C.texcoord, intersection.getU(), intersection.getV());
			info.color = interpolateColors(A.color, B.color, C.color, intersection.getU(), intersection.getV());
			info.material = material;
			return info;
		}
	}
}