#pragma once
#include "Ray.h"
#include "BoundingBox.h"
#include "Config.h"

#include <climits>

#define INFINITE_DISTANCE ((std::numeric_limits<float>::max)())


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
			virtual bool intersect(const tracer::Ray &ray, Intersection *intersection) const = 0;
			virtual HitInfo computeIntersection(const tracer::Ray &ray, const Intersection *intersection) const = 0;
			virtual BoundingBox computeBoundingBox() const = 0;
			virtual float area() const = 0;
			Material *material;
		};

		struct Intersection {
		private:
			float distance;
			const Hitable * hitable;
			// Interpolation
			float alpha;
			float beta;
		public:
			Intersection() : distance(INFINITE_DISTANCE), hitable(nullptr)
			{
			}

			bool isClosestThan(const Intersection &intersection)
			{
				if (distance < intersection.distance)
					return true;
				this->distance = intersection.distance;
				this->alpha = intersection.alpha;
				this->beta = intersection.beta;
				this->hitable = intersection.hitable;
				return false;
			}
			HitInfo compute(const tracer::Ray & ray) const
			{
				return this->hitable->computeIntersection(ray, this);
			}
			bool hit() const
			{
				return (this->hitable != nullptr);
			}

			bool set(float distance, const Hitable* hitable, float alpha = 0.f, float beta = 0.f)
			{
				this->distance = distance;
				this->hitable = hitable;
				this->alpha = alpha;
				this->beta = beta;
				return true;
			}

			point3 computeHit(const tracer::Ray &ray) const { return ray(distance); }
			float getAlpha() const { return alpha; }
			float getBeta() const { return beta; }
		};
	}
}