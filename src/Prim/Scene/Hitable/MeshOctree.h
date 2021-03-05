#pragma once

#include "Mesh.h"

#include <array>

namespace prim {

class MeshOctree final : public Mesh
{
public:
	struct Node {
		Node();
		Node(const Node &) = delete;
		Node &operator=(const Node &) = delete;
		~Node();

		// Check if the node is a leaf
		bool isLeaf() const { return childrens[0] == nullptr; }
		// Build the BVH
		uint32_t build(const std::vector<const Triangle*> &triangles, uint32_t depth);
		// Intersect the BVH
		bool intersect(const Ray &ray, Intersection *intersection) const;

		BoundingBox bbox;
		std::vector<const Triangle*> triangles;
		std::array<Node*, 8> childrens; // Childrens of the node
		point3f origin;                 // Center of the octree separating all childs
	};
public:
	MeshOctree(Material *material);
	MeshOctree(const MeshOctree &) = delete;
	MeshOctree &operator=(const MeshOctree &) = delete;

	// Build the BVH
	void build() override;
	// Intersect the BVH
	bool intersect(const Ray &ray, Intersection *intersection) const override;
private:
	Node m_rootNode;
};

}