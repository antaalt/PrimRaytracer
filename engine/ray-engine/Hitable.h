#pragma once
#include "Ray.h"

#include <climits>

#define INVALID_INTERSECTION std::numeric_limits<float>::max()


namespace app {
		
	namespace prim {

		class Material;

		struct HitInfo {
			Point3 point;
			Normal normal;
			Texcoord texcoord;
			ColorHDR color;
			Material *material;
		};

		struct Intersection {
			float distance;
			float beta; // interpolation
			float gamma; // interpolation
		};

		class Hitable
		{
		public:
			using Ptr = Hitable*;
			virtual bool intersect(const tracer::Ray &ray, Intersection &intersection) const = 0;

			virtual HitInfo computeIntersection(const tracer::Ray &ray, const Intersection &intersection) const = 0;
			Material *material;
		};
	}
}