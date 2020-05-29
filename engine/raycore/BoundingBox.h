#pragma once
#include "Config.h"
#include "Ray.h"

namespace raycore {

struct BoundingBox
{
	BoundingBox();
	BoundingBox(const geometry::point3f &min, const geometry::point3f &max);

	bool intersect(const Ray &ray) const;

	bool valid() const;

	float extent() const;
	geometry::point3f center() const;

	void include(const geometry::point3f &vec);
	void include(const BoundingBox &bbox);

	bool contain(const geometry::point3f &point) const;
	bool contain(const BoundingBox &bbox) const;

	bool overlap(const BoundingBox &bbox) const;

	geometry::point3f min, max;
};

}
