#pragma once
#include "Config.h"
#include "Ray.h"

namespace prim {

struct BoundingBox
{
	BoundingBox();
	BoundingBox(const point3f &min, const point3f &max);

	bool intersect(const Ray &ray) const;

	bool valid() const;

	vec3f extent() const;
	point3f center() const;

	void include(const point3f &vec);
	void include(const BoundingBox &bbox);

	bool contain(const point3f &point) const;
	bool contain(const BoundingBox &bbox) const;

	bool overlap(const BoundingBox &bbox) const;

	point3f min, max;
};

}
