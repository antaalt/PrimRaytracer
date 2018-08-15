#pragma once
#include "Ray.h"

#include <climits>

#define INVALID_INTERSECTION std::numeric_limits<float>::max()


namespace app {
		
	namespace prim {

		class Material;
		struct Intersection;

		struct HitInfo {
			point3 point;
			norm3 normal;
			uv2 texcoord;
			color4 color;
			Material *material;
		};

		class Hitable
		{
		public:
			using Ptr = Hitable*;
			virtual bool intersect(const tracer::Ray &ray, Intersection &intersection) const = 0;

			virtual HitInfo computeIntersection(const tracer::Ray &ray, const Intersection &intersection) const = 0;
			Material *material;
		};

		struct Intersection {
		private:
			float distance;
			const Hitable * hitable;
			// Interpolation
			float u;
			float v;
		public:
			Intersection() : distance(INVALID_INTERSECTION), hitable(nullptr)
			{
			}

			bool isClosestThan(const Intersection &intersection)
			{
				if (distance < intersection.distance)
					return true;
				this->distance = intersection.distance;
				this->u = intersection.u;
				this->v = intersection.v;
				this->hitable = intersection.hitable;
				return false;
			}
			HitInfo compute(const tracer::Ray & ray) const
			{
				return this->hitable->computeIntersection(ray, *this);
			}
			bool hit() const
			{
				return (this->hitable != nullptr);
			}

			bool set(float distance, const Hitable* hitable, float u = 0.f, float v = 0.f)
			{
				this->distance = distance;
				this->hitable = hitable;
				this->u = u;
				this->v = v;
				return true;
			}

			float getDistance() const { return distance; }
			float getU() const { return u; }
			float getV() const { return v; }
			//const Hitable *getHitable() const;
		};
	}
}