#include "BVH.h"
#include "Random.h"


namespace raycore {
	namespace prim {
		BVH::BVH(const std::vector<Hitable*> &prim) : Accelerator(prim)
		{
			Log::info("Building BVH...");
			this->root = new BVHNode(bbox.min, bbox.max);

			std::vector<const HitableBounded*> pHitableBounded;
			for (prim::Hitable* hitable : hitables) {
				pHitableBounded.push_back(new HitableBounded(hitable));
			}
			unsigned int nodeCount = this->root->init(pHitableBounded, 0);
			Log::info("BVH generated with ", nodeCount, " nodes (max. of ", (1 << (MAX_TREE_DEPTH + 1)), ") for ", this->hitables.size(), " hitables.");
			Log::info("BVH bounds : min: ", bbox.min, " - max: ", bbox.max);
		}
		BVH::~BVH()
		{
			delete root;
		}
		bool BVH::intersect(const tracer::Ray & ray, Intersection *intersection) const
		{
			return root->intersect(ray, intersection);
		}
		bool BVH::intersect(const tracer::Ray & ray) const
		{
			Intersection intersection;
			return intersect(ray, &intersection);
		}
		BVHNode::BVHNode(const point3 & min, const point3 & max) : BoundingBox(min, max), hitableCount(0)
		{
		}
		BVHNode::~BVHNode()
		{
			for(unsigned int i = 0; i < CHILD_COUNT; i++)
				delete childrens[i];
		}
		unsigned int BVHNode::init(const std::vector<const HitableBounded*>& hitableParents, unsigned int depth)
		{
			unsigned int nodeCount = 0;
			// 1. check the depth and add element
			if (hitableParents.size() < CHILD_MAX_HITABLE || depth > MAX_TREE_DEPTH)
			{
				//Log::debug("LEAF created - Depth : ", depth, ", contain ", hitableParent.size(), " hitables.");
				for(const HitableBounded* lHitable : hitableParents)
					this->hitable.push_back(lHitable->getHitable());
				this->hitableCount = this->hitable.size();
				ASSERT(isLeafNode(), "Should be a leaf");
				return 0;
			}
			// 2. set two random points as centroid
			size_t index[2];
			index[0] = rand::uirnd(0, static_cast<unsigned int>(hitableParents.size()) - 1);
			do {
				index[1] = rand::uirnd(0, static_cast<unsigned int>(hitableParents.size()) - 1);
			} while (index[1] == index[0]);
			point3 centroid[CHILD_COUNT] = {
				hitableParents[index[0]]->bbox().center(),
				hitableParents[index[1]]->bbox().center()
			};
			unsigned int loop = 0;
			std::vector<const HitableBounded*> subGroup[CHILD_COUNT];
			float epsilon[CHILD_COUNT];
			do {
				subGroup[0].clear();
				subGroup[1].clear();
				// 3. group by closest
				for (const HitableBounded* hitableParent : hitableParents)
				{
					const prim::BoundingBox &bbox = hitableParent->bbox();
					const point3 c = bbox.center();
					ASSERT(contain(c), "Should be inside");
					const float dist[2] = {
						distance(c, centroid[0]),
						distance(c, centroid[1])
					};
					if (dist[0] > dist[1])
						subGroup[1].push_back(hitableParent);
					else if (dist[0] < dist[1])
						subGroup[0].push_back(hitableParent);
					else // equal, fill an empty node
					{
						if (subGroup[0].size() == 0)
							subGroup[0].push_back(hitableParent);
						else
							subGroup[1].push_back(hitableParent);
					}
				}
				// 4. set two points as center of groups
				for (unsigned int iGroup = 0; iGroup < CHILD_COUNT; iGroup++)
				{
					ASSERT(subGroup[iGroup].size() > 0, "Should not be empty");
					point3 tmp(0.f);
					float weights = 0.f;
					for (const HitableBounded* hitable : subGroup[iGroup])
					{
						float weight = hitable->getHitable()->area();
						tmp += hitable->bbox().center() * weight;
						weights += weight;
					}
					point3 newGroupCentroid = tmp / weights;
					// Check weights
					epsilon[iGroup] = distance(newGroupCentroid, centroid[iGroup]);
					centroid[iGroup] = newGroupCentroid;
				}
				// 5. redo from 3 until difference small enough
			} while ((epsilon[0] > EPSILON_LIMIT && epsilon[1] > EPSILON_LIMIT) || loop++ > MAX_KMEAN_DEPTH);

			// 6. compute bbox of groups and create childrens
			for (unsigned int iGroup = 0; iGroup < CHILD_COUNT; iGroup++)
			{
				prim::BoundingBox containers;
				for (const HitableBounded* &hitable : subGroup[iGroup])
					containers.include(hitable->bbox());
				ASSERT(subGroup[iGroup].size() > 0, "Should not be empty");
				ASSERT(contain(containers), "Should contain container");
				childrens[iGroup] = new BVHNode(containers.min, containers.max);
				nodeCount += childrens[iGroup]->init(subGroup[iGroup], depth + 1);
				nodeCount++;
			}
			return nodeCount;
		}
		bool BVHNode::intersect(const tracer::Ray & ray, Intersection * intersection)
		{
			if (!this->intersectBounds(ray))
				return false;
			if (this->isLeafNode())
			{
				for (size_t iTri = 0; iTri < this->hitableCount; iTri++)
				{
					Intersection localIntersection;
					if (this->hitable[iTri]->intersect(ray, &localIntersection))
						intersection->isClosestThan(localIntersection);
				}
				return intersection->hit();
			}
			else
			{
				for (unsigned int i = 0; i < CHILD_COUNT; i++)
				{
					Intersection localIntersection;
					if (childrens[i]->intersect(ray, &localIntersection))
						intersection->isClosestThan(localIntersection);
				}
				return intersection->hit();
			}
			return false;
		}
		bool BVHNode::isLeafNode()
		{
			return hitableCount != 0;
		}
	}
}