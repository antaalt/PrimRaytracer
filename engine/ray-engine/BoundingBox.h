#pragma once
#include "Config.h"
#include "Hitable.h"

namespace app {

	namespace prim {

		class BoundingBox
		{
		public:
			BoundingBox();
			BoundingBox(const Point3 &min, const Point3 &max);
			~BoundingBox();

			bool intersect(const tracer::Ray &ray) const;

			float extent() const;
			Point3 center() const;

			void include(const Point3 &vec);
			void include(const BoundingBox &bbox);

			void reset();

			Point3 min, max;
		};
	}
}
