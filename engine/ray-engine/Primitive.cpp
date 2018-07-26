#include "Primitive.h"


namespace Application {
	namespace RayTracer {

		Primitive::Primitive()
		{
		}


		Primitive::~Primitive()
		{


		}
		bool Primitive::intersect(const Ray & ray, Intersection & intersection) const
		{
			Intersection testIntersection;
			for (size_t iTri = 0; iTri < triangles.size(); iTri++)
			{
				const Triangle &tri = triangles[iTri];
				const Vertex A = vertices[tri.A];
				const Vertex B = vertices[tri.B];
				const Vertex C = vertices[tri.C];
				// compute plane's geometric normal
				Vector3 AB = B.position - A.position;
				Vector3 AC = C.position - A.position;
				Vector3 BC = C.position - B.position;
				Vector3 CA = A.position - C.position;
				Vector3 N = Vector3::cross(AB, AC);
				float area2 = N.length();

				float NdotRayDirection = Vector3::dot(N, ray.direction);
				if (fabs(NdotRayDirection) < EPSILON)
					continue;

				float d = Vector3::dot(N, A.position);
				float t = (Vector3::dot(N, ray.origin) + d) / NdotRayDirection;
				if (t < 0 || t > testIntersection.distance)
					continue;

				// Check if hit point is inside triangle
				Vector3 P = ray.origin + t * ray.direction;
				Vector3 crosse;
				Vector3 vp0 = P - A.position;
				crosse = Vector3::cross(AB, vp0);
				if (Vector3::dot(N, crosse) < 0)
					continue;

				Vector3 vp1 = P - B.position;
				crosse = Vector3::cross(BC, vp1);
				if (Vector3::dot(N, crosse) < 0)
					continue;

				Vector3 vp2 = P - C.position;
				crosse = Vector3::cross(CA, vp2);
				if (Vector3::dot(N, crosse) < 0)
					continue;

				float totalLength = vp0.length() + vp1.length() + vp2.length();
				float aRatio = vp0.length() / totalLength;
				float bRatio = vp1.length() / totalLength;
				float cRatio = vp2.length() / totalLength;

				testIntersection.hit = true;
				testIntersection.distance = t;
				testIntersection.point = P;
				testIntersection.normal = Normal::normalize(A.normal * aRatio + B.normal * bRatio + C.normal * cRatio);
				testIntersection.texCoord = A.texcoord * aRatio + B.texcoord * bRatio + C.texcoord * cRatio;

				testIntersection.material = material;				
			}
			if (testIntersection.distance < std::numeric_limits<float>::max())
			{
				intersection = testIntersection;
				return true;
			}
			else
				return false;
		}
		Triangle::Triangle()
		{
		}
		Triangle::Triangle(unsigned int v1, unsigned int v2, unsigned int v3) : A(v1), B(v2), C(v3)
		{
		}
	}
}