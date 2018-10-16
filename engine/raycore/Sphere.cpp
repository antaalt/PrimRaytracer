#include "Sphere.h"
#include "Onb.h"

namespace raycore {
	namespace prim {

		Sphere::Sphere()
		{
		}

		Sphere::Sphere(const point3 & center, float radius, const vec3 up) : center(center), radius(radius), up(up)
		{
		}

		Sphere::~Sphere()
		{
		}

		bool Sphere::intersect(const tracer::Ray & ray, Intersection & intersection) const
		{
			vec3 eyeDirection = this->center - ray.origin;
			float b = vec3::dot(eyeDirection, ray.direction); // a is always 1 because dot of same normalized vector
			float det = b * b - vec3::dot(eyeDirection, eyeDirection) + this->radius * this->radius;
			if (det < 0)
				return false;
			else
				det = sqrt(det);

			float t1 = b - det;
			float t2 = b + det;
			if (t1 > ray.tmin && t1 < ray.tmax)
				return intersection.set(t1, this);
			else if (t2 > ray.tmin && t2 < ray.tmax)
				return intersection.set(t2, this);
			else
				return false;
		}

		HitInfo Sphere::computeIntersection(const tracer::Ray & ray, const Intersection & intersection) const
		{
			HitInfo info;
			info.direction = ray.direction;
			info.point = ray.origin + ray.direction * intersection.getDistance();
			info.normal = vec3::normalize(info.point - this->center);
			// https://en.wikipedia.org/wiki/UV_mraycoreing
			transform::Onb onb(this->up);
			norm3 n = onb(info.normal);
			float u = 0.5f + atan2(-n.z, -n.x) / (2.f * M_PIf);
			float v = 0.5f - asin(-n.y) / M_PIf;
			info.texcoord = uv2(u, v);
			info.color = colorHDR(1.f);
			info.material = material;
			return info;
		}

		BoundingBox Sphere::computeBoundingBox() const
		{
			BoundingBox bbox;
			bbox.min = this->center - vec3(this->radius);
			bbox.max = this->center + vec3(this->radius);
			return bbox;
		}

	}
}