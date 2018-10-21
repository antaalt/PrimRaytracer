#include "Triangle.h"


namespace raycore {
	namespace prim {


		Vertex::Vertex()
		{
		}
		Vertex::Vertex(point3 p, norm3 n, uv2 tc, colorHDR c) : position(p), normal(n), texcoord(tc), color(c)
		{
		}

		Triangle::Triangle()
		{
		}
		Triangle::Triangle(const Vertex & vA, const Vertex & vB, const Vertex & vC) :
			A(vA), B(vB), C(vC)
		{
		}
		bool Triangle::intersect(const tracer::Ray & ray, Intersection & intersection) const
		{
#if defined(MOLLER_TRUMBORE)
			// https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
			const float epsilon = 0.0000001f;
			vec3 AB(B.position - A.position);
			vec3 AC(C.position - A.position);
			vec3 h = cross(ray.direction, AC);
			float a = dot(AB, h);
#if defined(BACKFACE_CULLING)
			if (a < epsilon) 
				return false;
#else 
			if (fabs(a) < epsilon)
				return false;
#endif 
			float f = 1.f / a;
			
			vec3 s(ray.origin - A.position);
			float u = f * dot(s, h);
			if (u < 0.f || u > 1.f)
				return false;
			vec3 q = cross(s, AB);
			float v = f * dot(ray.direction, q);
			if (v < 0.f || u + v > 1.f)
				return false;
			float t = f * dot(AC, q);
			if (t > epsilon && t > ray.tmin && t < ray.tmax)
				return intersection.set(t, this, u, v);
			else
				return false;
#else 
#error "not working"
			const vec3 AB = B.position - A.position;
			const vec3 AC = C.position - A.position;
			const normal geonormal = vec3::cross(AB, AC);
			float denom = vec3::dot(geonormal, geonormal);

			float NdotRD = vec3::dot(geonormal, ray.direction);
#if defined(BACKFACE_CULLING)
#else
			if (fabs(NdotRD) < EPSILON)
				return false;
#endif

			float d = vec3::dot(geonormal, A.position);

			float t = (vec3::dot(geonormal, ray.origin) + d) / NdotRD;
			if (t < 0)
				return false;

			float u, v;
			vec3 P = ray.origin + t * ray.direction;

			vec3 C2;

			vec3 AP = P - A.position;
			C2 = vec3::cross(AB, AP);
			if (vec3::dot(geonormal, C2) < 0)
				return false;

			vec3 BC = C.position - B.position;
			vec3 BP = P - B.position;
			C2 = vec3::cross(BC, BP);
			if ((u = vec3::dot(geonormal, C2)) < 0)
				return false;

			vec3 CA = A.position - C.position;
			vec3 CP = P - C.position;
			C2 = vec3::cross(CA, CP);
			if ((v = vec3::dot(geonormal, C2)) < 0)
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

		inline norm3 interpolatenormal(const norm3 &nA, const norm3 &nB, const norm3 &nC, float alpha, float beta)
		{
			return normalize(norm3(nB * alpha + nC * beta + nA * (1.f - alpha - beta)));
		}
		inline uv2 interpolateTexCoord(const uv2 &tA, const uv2 &tB, const uv2 &tC, float alpha, float beta)
		{
			return tB * alpha + tC * beta + tA * (1.f - alpha - beta);
		}
		inline colorHDR interpolateColors(const colorHDR &cA, const colorHDR &cB, const colorHDR &cC, float alpha, float beta)
		{
			return cB * alpha + cC * beta + cA * (1.f - alpha - beta);
		}

		HitInfo Triangle::computeIntersection(const tracer::Ray & ray, const Intersection & intersection) const
		{
			HitInfo info;
			info.direction = ray.direction;
			info.point = point3(ray.origin + ray.direction * intersection.getDistance());
			float alpha = intersection.getAlpha();
			float beta = intersection.getBeta();
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
	}
}