#pragma once
#include "Config.h"
#include "Hitable.h"

//#define BRANCHLESS_INTERSECTION

namespace app {

	namespace prim {

		class BoundingBox
		{
		public:
			BoundingBox();
			BoundingBox(const point3 &min, const point3 &max);
			~BoundingBox();

			bool intersectBounds(const tracer::Ray &ray) const;

			float extent() const;
			point3 center() const;

			void include(const point3 &vec);
			void include(const BoundingBox &bbox);

			bool contain(const point3 &point) const;
			bool contain(const BoundingBox &bbox) const;

			bool overlap(const BoundingBox &bbox) const;

			void reset();

			point3 min, max;
		};
	}
}
