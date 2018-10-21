#include "Parallelogram.h"

namespace raycore {
	namespace prim {

		Parallelogram::Parallelogram()
		{
		}

		Parallelogram::Parallelogram(const point3 & A, const point3 & B, const point3 & C, const norm3 & normal)
		{
			this->point[0] = A;
			this->point[1] = B;
			this->point[2] = C;
			point3 bisect = ((this->point[0] + this->point[2]) / 2.f - this->point[1] * 2.f);
			this->point[3] = this->point[1] + bisect;
			this->normal = normal;
		}


		Parallelogram::~Parallelogram()
		{
		}
		bool Parallelogram::intersect(const tracer::Ray & ray, Intersection & intersection) const
		{
			float denom = dot(ray.direction, this->normal);
			if (fabs(denom) < EPSILON)
				return false;
			float t = dot(this->point[0] - ray.origin, this->normal) / denom;
			if (t < 0.f)
				return false;
			/*point3 hitPoint = ray.origin + ray.direction * t;
			vec3::cross(this->point[0], this->point[1]);
			vec3::cross(this->point[1], this->point[2]);
			vec3::cross(this->point[2], this->point[3]);
			vec3::cross(this->point[1], this->point[2]);*/
			intersection.set(t, this);
			return true;
		}
		HitInfo Parallelogram::computeIntersection(const tracer::Ray & ray, const Intersection & intersection) const
		{
			HitInfo info;
			info.direction = ray.direction;
			info.point = point3(ray.origin + ray.direction * intersection.getDistance());
			info.normal = this->normal;
			info.texcoord = uv2(); // TODO compute texcoord
			info.color = colorHDR(1.f);
			info.material = material;
			return info;
		}
		BoundingBox Parallelogram::computeBoundingBox() const
		{
			return BoundingBox();
		}
	}
}


