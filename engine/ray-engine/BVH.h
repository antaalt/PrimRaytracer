#pragma once
#include "Accelerator.h"
#include "BoundingBox.h"

#define CHILD_COUNT 2
#define MAX_TREE_DEPTH 15
#define MAX_KMEAN_DEPTH 15
#define EPSILON_LIMIT 0.001f
//#define USE_HITABLE_BBOX_INTERSECTION

// @see https://github.com/brandonpelfrey/Fast-BVH/blob/master/BVH.h

namespace app {
	namespace tracer {

		struct HitableBounded {
			HitableBounded(prim::Hitable * hitable) : hitable(hitable) {
				this->boundingBox = hitable->computeBoundingBox();
			}
			const prim::BoundingBox &bbox() const { return boundingBox; }
			bool intersect(const Ray &ray, prim::Intersection &intersection) const
			{ 
#if defined(USE_HITABLE_BBOX_INTERSECTION)
				if (!boundingBox.intersectBounds(ray))
					return false;
#endif
				return hitable->intersect(ray, intersection);
			}
		private:
			prim::Hitable *hitable;
			prim::BoundingBox boundingBox;
		};



		struct BVHNode : public prim::BoundingBox {
			BVHNode(const vec3 &min, const vec3 &max);
			~BVHNode();

			unsigned int init(const std::vector<const HitableBounded*> &hitables, unsigned int depth);

			bool intersect(const Ray &ray, prim::Intersection &intersection);

			bool isLeafNode();
		private:
			BVHNode *childrens[CHILD_COUNT];
			std::vector<const HitableBounded*> hitableBounded; // TODO stock hitable directly
		};



		class BVH :public Accelerator
		{
		public:
			BVH();
			~BVH();
			BVH(const BVH& other) = delete;
			BVH& operator=(const BVH &other) = delete;

			virtual bool build(const Scene &scene);
			virtual bool intersect(const Ray &ray, prim::HitInfo &info) const;
			virtual bool isOccluded(const Ray &ray) const;

		private:
			BVHNode *root;
			std::vector<HitableBounded> hitableBounded;
		};
	}
}

