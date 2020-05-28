#pragma once
#include "Accelerator.h"
#include "BoundingBox.h"

#define CHILD_COUNT 2 // only 2 supported now
#define MAX_TREE_DEPTH 20
#define MAX_KMEAN_DEPTH 15
#define CHILD_MAX_HITABLE 5
#define EPSILON_LIMIT 0.001f

// @see https://github.com/brandonpelfrey/Fast-BVH/blob/master/BVH.h

namespace raycore {
namespace prim {

struct HitableBounded {
	HitableBounded(Hitable * hitable) : hitable(hitable) {
		this->boundingBox = hitable->computeBoundingBox();
	}
	const BoundingBox &bbox() const { return boundingBox; }
	const Hitable *getHitable() const { return hitable; }
private:
	Hitable *hitable;
	BoundingBox boundingBox;
};



struct BVHNode : public BoundingBox {
	BVHNode(const geometry::point3f &min, const geometry::point3f &max);
	~BVHNode();

	unsigned int init(const std::vector<const HitableBounded*> &hitables, unsigned int depth);

	bool intersect(const tracer::Ray &ray, Intersection *intersection);

	bool isLeafNode();
private:
	BVHNode *childrens[CHILD_COUNT];
	std::vector<const Hitable*> hitable;
	size_t hitableCount;
};



class BVH : public Accelerator
{
public:
	BVH(const std::vector<Hitable*> &prim);
	~BVH();
	virtual bool intersect(const tracer::Ray &ray, Intersection *info) const;
	virtual bool intersect(const tracer::Ray &ray) const;

private:
	BVHNode *root;
};
}
}

