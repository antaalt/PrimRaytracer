#include "BoundingBox.h"

#include <climits>

namespace Application::RayTracer {

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
	bool BoundingBox::intersect(const Ray & ray, Intersection & intersection) const
	{
		// TODO implement intersect bounding box
		/*float tmin, tmax, tymin, tymax, tzmin, tzmax;

		tmin = (bounds[r.sign[0]].x - r.orig.x) * r.invdir.x;
		tmax = (bounds[1 - r.sign[0]].x - r.orig.x) * r.invdir.x;
		tymin = (bounds[r.sign[1]].y - r.orig.y) * r.invdir.y;
		tymax = (bounds[1 - r.sign[1]].y - r.orig.y) * r.invdir.y;

		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;

		tzmin = (bounds[r.sign[2]].z - r.orig.z) * r.invdir.z;
		tzmax = (bounds[1 - r.sign[2]].z - r.orig.z) * r.invdir.z;

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		t = tmin;

		if (t < 0) {
			t = tmax;
			if (t < 0) return false;
		}

		return true;*/
		return false;
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
