#include "Triangle.h"


namespace Application::RayTracer {

	Triangle::Triangle()
	{
	}


	Triangle::~Triangle()
	{
	}
	bool Triangle::intersect(const Ray & ray, Intersection & intersection) const
	{
		// compute plane's normal
		Vector3 AB = B - A;
		Vector3 AC = C - A;
		Vector3 BC = C - B;
		Vector3 CA = A - C;
		// no need to normalize
		Vector3 N = cross(AB, AC);
		float area2 = N.length();

		// Step 1: finding P

		// check if ray and plane are parallel ?
		float NdotRayDirection = dot(N, ray.direction);
		if (fabs(NdotRayDirection) < EPSILON) // almost 0 
			return false; // they are parallel so they don't intersect ! 

						  // compute d parameter using equation 2
		float d = dot(N, A);

		// compute t (equation 3)
		float t = (dot(N, ray.origin) + d) / NdotRayDirection;
		// check if the triangle is in behind the ray
		if (t < 0) return false; // the triangle is behind 

								 // compute the intersection point using equation 1
		Vector3 P = ray.origin + t * ray.direction;

		// Step 2: inside-outside test
		Vector3 C; // vector perpendicular to triangle's plane 

		// edge 0
		Vector3 vp0 = P - A;
		C = cross(AB, vp0);
		if (dot(N, C) < 0) return false; // P is on the right side 

		// edge 1
		Vector3 vp1 = P - B;
		C = cross(BC, vp1);
		if (dot(N, C) < 0)  return false; // P is on the right side 

		// edge 2
		Vector3 vp2 = P - C;
		C = cross(CA, vp2);
		if (dot(N, C) < 0) return false; // P is on the right side; 

		return true; // this ray hits the triangle
	}
}