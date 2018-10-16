#pragma once
#include "Ray.h"
#include "BoundingBox.h"

#include <climits>

#define INFINITE_DISTANCE (std::numeric_limits<float>::max())


namespace raycore {
		
	namespace prim {

		class Material;

		struct HitInfo {
			vec3 direction;
			point3 point;
			norm3 normal;
			uv2 texcoord;
			colorHDR color;
			Material *material;
		};

		struct Intersection;

		class Hitable
		{
		public:
			virtual bool intersect(const tracer::Ray &ray, Intersection &intersection) const = 0;
			virtual HitInfo computeIntersection(const tracer::Ray &ray, const Intersection &intersection) const = 0;
			virtual BoundingBox computeBoundingBox() const = 0;

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
			Intersection() : distance(INFINITE_DISTANCE), hitable(nullptr)
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
		};
	}
}