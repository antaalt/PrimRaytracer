#include "Octree.h"


namespace app {
	namespace tracer {

		Octree::Octree()
		{
		}


		Octree::~Octree()
		{
		}

		bool Octree::build(const Scene & scene)
		{
			return false;
		}

		bool Octree::intersect(const Ray & ray, prim::HitInfo &info) const
		{
			return false;
		}

		prim::HitInfo Octree::computeIntersection(const tracer::Ray & ray, const prim::Intersection & intersection) const
		{
			return prim::HitInfo();
		}

		bool Octree::add(const Primitive & primitive)
		{

			return false;
		}

	}
}