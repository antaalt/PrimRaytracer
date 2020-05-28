#pragma once
#include "Config.h"
#include "Ray.h"

//#define BRANCHLESS_INTERSECTION

namespace raycore {

namespace prim {

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(const geometry::point3f &min, const geometry::point3f &max);

	bool intersectBounds(const tracer::Ray &ray) const;

	float extent() const;
	geometry::point3f center() const;

	void include(const geometry::point3f &vec);
	void include(const BoundingBox &bbox);

	bool contain(const geometry::point3f &point) const;
	bool contain(const BoundingBox &bbox) const;

	bool overlap(const BoundingBox &bbox) const;

	void reset();

	geometry::point3f min, max;
};
}
}
