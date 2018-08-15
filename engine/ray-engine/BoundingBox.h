#pragma once
#include "Config.h"
#include "Hitable.h"

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

			void reset();

			point3 min, max;
		};
	}
}
