#include "BoundingBox.h"
#include "Math.h"
#include <climits>

namespace app {

	namespace prim {

		BoundingBox::BoundingBox()
		{
			reset();
		}

		BoundingBox::BoundingBox(const point3 &min, const point3 &max) : min(min), max(max)
		{
		}


		BoundingBox::~BoundingBox()
		{
		}
		bool BoundingBox::intersectBounds(const tracer::Ray & ray) const
		{
#if defined(BRANCHLESS_INTERSECTION) // @see https://tavianator.com/fast-branchless-raybounding-box-intersections/
			double tx1 = (min.x - ray.origin.x) * -ray.direction.x;
			double tx2 = (max.x - ray.origin.x) * -ray.direction.x;

			double tmin = app::min(tx1, tx2);
			double tmax = app::max(tx1, tx2);

			double ty1 = (min.y - ray.origin.y) * -ray.direction.y;
			double ty2 = (max.y - ray.origin.y) * -ray.direction.y;

			tmin = app::max(tmin, app::min(ty1, ty2));
			tmax = app::min(tmax, app::max(ty1, ty2));

			return tmax >= tmin;
#else
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
#endif
		}
		float BoundingBox::extent() const
		{
			return (max - min).length();
		}
		point3 BoundingBox::center() const
		{
			return (max + min) / 2.f;
		}
		void BoundingBox::include(const point3 & vec)
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
		bool BoundingBox::contain(const point3 & point) const
		{
			return (point.x <= max.x &&
				point.y <= max.y &&
				point.z <= max.z &&
				point.x >= min.x &&
				point.y >= min.y &&
				point.z >= min.z);
		}
		bool BoundingBox::contain(const BoundingBox & bbox) const
		{
			return (bbox.max.x <= max.x &&
				bbox.max.y <= max.y &&
				bbox.max.z <= max.z &&
				bbox.min.x >= min.x &&
				bbox.min.y >= min.y &&
				bbox.min.z >= min.z);
		}
		void BoundingBox::reset()
		{
			min = point3(std::numeric_limits<float>::max());
			max = point3(std::numeric_limits<float>::min());
		}
	}
}
