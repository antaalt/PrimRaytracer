#include "BoundingBox.h"
#include "Config.h"
#include <climits>

namespace raycore {

BoundingBox::BoundingBox() :
	BoundingBox(geometry::point3f(std::numeric_limits<float>::max()), geometry::point3f(-std::numeric_limits<float>::max()))
{
}

BoundingBox::BoundingBox(const geometry::point3f &min, const geometry::point3f &max) : min(min), max(max)
{
}

bool BoundingBox::intersect(const Ray & ray) const
{
#if defined(BRANCHLESS_INTERSECTION) // @see https://tavianator.com/fast-branchless-raybounding-box-intersections/
	double tx1 = (min.x - ray.origin.x) * -ray.direction.x;
	double tx2 = (max.x - ray.origin.x) * -ray.direction.x;

	double tmin = raycore::min(tx1, tx2);
	double tmax = raycore::max(tx1, tx2);

	double ty1 = (min.y - ray.origin.y) * -ray.direction.y;
	double ty2 = (max.y - ray.origin.y) * -ray.direction.y;

	tmin = raycore::max(tmin, raycore::min(ty1, ty2));
	tmax = raycore::min(tmax, raycore::max(ty1, ty2));

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

bool BoundingBox::valid() const
{
	return (min.x < max.x && min.y < max.y && min.z < max.z);
}

float BoundingBox::extent() const
{
	return geometry::vec3f(max - min).norm();
}

geometry::point3f BoundingBox::center() const
{
	return (max + min) / 2.f;
}

void BoundingBox::include(const geometry::point3f & vec)
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

bool BoundingBox::contain(const geometry::point3f & point) const
{
	return (
		point.x <= max.x &&
		point.y <= max.y &&
		point.z <= max.z &&
		point.x >= min.x &&
		point.y >= min.y &&
		point.z >= min.z
	);
}

bool BoundingBox::contain(const BoundingBox & bbox) const
{
	return contain(bbox.min) && contain(bbox.max);
}

bool BoundingBox::overlap(const BoundingBox & bbox) const
{
	return (bbox.min.x <= max.x &&
		bbox.min.y <= max.y &&
		bbox.min.z <= max.z) &&
		(bbox.max.x >= min.x &&
		bbox.max.y >= min.y &&
		bbox.max.z >= min.z);
}

}
