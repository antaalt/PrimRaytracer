#include "Parallelogram.h"

namespace app {
	namespace prim {

		Parallelogram::Parallelogram()
		{
		}

		Parallelogram::Parallelogram(const Point3 & A, const Point3 & B, const Point3 & C, const Normal & normal)
		{
			this->point[0] = A;
			this->point[1] = B;
			this->point[2] = C;
			Point3 bisect = ((this->point[0] + this->point[2]) / 2.f - this->point[1] * 2.f);
			this->point[3] = this->point[1] + bisect;
			this->normal = normal;
		}


		Parallelogram::~Parallelogram()
		{
		}
		bool Parallelogram::intersect(const tracer::Ray & ray, Intersection & intersection) const
		{
			float denom = Vector3::dot(this->normal, ray.direction);
			if (fabs(denom) < EPSILON)
				return false;
			float t = Vector3::dot(this->point[0] - ray.origin, this->normal) / denom;
			if (t < 0.f)
				return false;
			/*Point3 hitPoint = ray.origin + ray.direction * t;
			Vector3::cross(this->point[0], this->point[1]);
			Vector3::cross(this->point[1], this->point[2]);
			Vector3::cross(this->point[2], this->point[3]);
			Vector3::cross(this->point[1], this->point[2]);*/
			intersection.set(t, this);
			return true;
		}
		HitInfo Parallelogram::computeIntersection(const tracer::Ray & ray, const Intersection & intersection) const
		{
			HitInfo info;
			info.point = ray.origin + ray.direction * intersection.getDistance();
			info.normal = this->normal;
			info.texcoord = Texcoord(); // TODO compute texcoord
			info.color = ColorHDR(1.f);
			info.material = material;
			return info;
		}
	}
}


