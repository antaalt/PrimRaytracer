#include "BoundingBox.h"

#include <climits>

namespace app {

	namespace prim {

		BoundingBox::BoundingBox()
		{
			reset();
		}

		BoundingBox::BoundingBox(const Point3 &min, const Point3 &max) : min(min), max(max)
		{
		}


		BoundingBox::~BoundingBox()
		{
		}
		bool BoundingBox::intersect(const tracer::Ray & ray) const
		{
			float tmin = (min.x - ray.origin.x) / ray.direction.x;
			float tmax = (max.x - ray.origin.x) / ray.direction.x;

			if (tmin > tmax) std::swap(tmin, tmax);

			float tymin = (min.y - ray.origin.y) / ray.direction.y;
			float tymax = (max.y - ray.origin.y) / ray.direction.y;

			if (tymin > tymax) std::swap(tymin, tymax);

			if ((tmin > tymax) || (tymin > tmax))
				return false;

			if (tymin > tmin)
				tmin = tymin;

			if (tymax < tmax)
				tmax = tymax;

			float tzmin = (min.z - ray.origin.z) / ray.direction.z;
			float tzmax = (max.z - ray.origin.z) / ray.direction.z;

			if (tzmin > tzmax) std::swap(tzmin, tzmax);

			if ((tmin > tzmax) || (tzmin > tmax))
				return false;

			if (tzmin > tmin)
				tmin = tzmin;

			if (tzmax < tmax)
				tmax = tzmax;

			return true;
		}
		float BoundingBox::extent() const
		{
			return (max - min).length();
		}
		Point3 BoundingBox::center() const
		{
			return (max + min) / 2.f;
		}
		void BoundingBox::include(const Point3 & vec)
		{
			if (min.x > vec.x) min.x = vec.x;
			if (min.y > vec.y) min.y = vec.y;
			if (min.z > vec.z) min.z = vec.z;
			if (max.x < vec.x) max.x = vec.x;
			if (max.y < vec.y) max.y = vec.y;
			if (max.z < vec.z) max.z = vec.z;
		}
		void BoundingBox::include(const BoundingBox & bbox)
		{
			include(bbox.max);
			include(bbox.min);
		}
		void BoundingBox::reset()
		{
			min = Point3(std::numeric_limits<float>::max());
			max = Point3(std::numeric_limits<float>::min());
		}
	}
}
