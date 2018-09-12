#include "Octree.h"
#include "Material.h"
#include <map>
namespace app {
	namespace tracer {

		Octree::Octree()
		{
		}

		Octree::~Octree()
		{
			for (size_t i = 0; i < this->hitables.size(); i++)
				delete this->hitables[i];
			for (size_t i = 0; i < this->materials.size(); i++)
				delete this->materials[i];
			delete root;
		}

		bool Octree::build(const Scene & scene)
		{
			Accelerator::build(scene);
			this->root = new OctNode(bbox.min, bbox.max);
			std::vector<const prim::Triangle*> triangles;
			for (auto it = this->hitables.begin(); it != this->hitables.end(); it++)
			{
				// Check if hitable is a triangle
				const prim::Triangle* tri = dynamic_cast<prim::Triangle*>((*it));
				if (tri != nullptr)
					triangles.push_back(tri);
			}
			unsigned int created = this->root->init(triangles);
			Log::debug("Octree generated with ", created, " subtrees");
			return created > 0;
		}

		bool Octree::intersect(const Ray & ray, prim::HitInfo &info) const
		{
			prim::Intersection intersection;
			if (!root->intersect(ray, intersection))
				return false;
			info = intersection.compute(ray);
			return true;
		}

		bool Octree::intersect(const Ray & ray) const
		{
			prim::Intersection intersection;
			return root->intersect(ray, intersection);
		}

		bool OctNode::isLeafNode() const
		{
			for (int i = 0; i < OCTREE_CHILD_NODE; i++)
				if (this->childrens[i] != nullptr)
					return false;
			return true;
		}

		unsigned int OctNode::init(std::vector<const prim::Triangle*> &parentTri, unsigned int depth)
		{
			if (depth >= MAX_OCTREE_DEPTH || parentTri.size() <= MAX_OCTREE_CHILD)
			{
				this->triangles = parentTri;
				ASSERT(isLeafNode(), "Should be a leaf");
				return 0;
			}
			// Get optimal center of bbox
#if 0
			point3 c(0.f);
			for (auto it = parentTri.begin(); it != parentTri.end(); it++)
				c = c + ((*it)->A.position + (*it)->B.position + (*it)->C.position) / 3.f;
			c = c / static_cast<float>(parentTri.size());
			ASSERT(contain(c), "Not inside");
#else
			origin = center();
#endif
			unsigned int created = 0;
			std::vector<const prim::Triangle*> childTri[OCTREE_CHILD_NODE];
			for (auto it = parentTri.begin(); it != parentTri.end(); it++)
			{
				unsigned int octA = getOctant((*it)->A.position);
				unsigned int octB = getOctant((*it)->B.position);
				unsigned int octC = getOctant((*it)->C.position);
				childTri[octA].push_back((*it));
				if (octA != octB)
					childTri[octB].push_back((*it));
				if (octA != octC)
					childTri[octC].push_back((*it));
			}
			for (int iChild = 0; iChild < OCTREE_CHILD_NODE; iChild++)
			{
				if (childTri[iChild].size() == 0)
					continue;
				// Compute new bounding box for this child
				prim::BoundingBox childBox;
				childBox.min.x = (iChild & (1 << 2)) ? origin.x : min.x;
				childBox.max.x = (iChild & (1 << 2)) ? max.x : origin.x;
				childBox.min.y = (iChild & (1 << 1)) ? origin.y : min.y;
				childBox.max.y = (iChild & (1 << 1)) ? max.y : origin.y;
				childBox.min.z = (iChild & (1 << 0)) ? origin.z : min.z;
				childBox.max.z = (iChild & (1 << 0)) ? max.z : origin.z;
				ASSERT(contain(childBox), "Not inside");

				this->childrens[iChild] = new OctNode(childBox.min, childBox.max);
				created += this->childrens[iChild]->init(childTri[iChild], depth + 1);
				created++;
			}
			return created;
		}

		OctNode::OctNode() : OctNode(point3(0.f), point3(1.f))
		{
		}

		OctNode::OctNode(const point3 & min, const point3 & max) : BoundingBox(min, max)
		{
			for (unsigned int i = 0; i < OCTREE_CHILD_NODE; i++)
				this->childrens[i] = nullptr;
		}

		OctNode::~OctNode()
		{
			for (unsigned int i = 0; i < OCTREE_CHILD_NODE; i++)
				delete childrens[i];
		}

		bool OctNode::intersect(const tracer::Ray & ray, prim::Intersection & intersection) const
		{
			if (!this->intersectBounds(ray))
				return false;
			if (this->isLeafNode())
			{
				ASSERT(this->triangles.size() > 0, "what");
				for (size_t iTri = 0; iTri < this->triangles.size(); iTri++)
				{
					prim::Intersection localIntersection;
					if (this->triangles[iTri]->intersect(ray, localIntersection))
						intersection.isClosestThan(localIntersection);
				}
				return intersection.hit();
			}
			else
			{
				for (unsigned int i = 0; i < OCTREE_CHILD_NODE; i++)
				{
					prim::Intersection localIntersection;
					if (childrens[i] == nullptr)
						continue;
					if (childrens[i]->intersect(ray, localIntersection))
						intersection.isClosestThan(localIntersection);
				}
				return intersection.hit();
			}
		}

		unsigned int OctNode::getOctant(const vec3 & point) const
		{
			unsigned int oct = 0;
			if (point.x >= origin.x) oct |= (1 << 2);
			if (point.y >= origin.y) oct |= (1 << 1);
			if (point.z >= origin.z) oct |= (1 << 0);
			return oct;
		}
	}
}