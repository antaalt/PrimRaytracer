#pragma once

#include <vector>
#include <array>

#include "Mesh.h"
#include "../../Core/BoundingBox.h"

namespace prim {

class MeshBVH final : public Mesh {
public:
	struct Node {
		Node();
		Node(const Node &) = delete;
		Node &operator=(const Node &) = delete;
		~Node();

		// Check if the node is a leaf
		bool isLeaf() const { return triangles.size() != 0; }
		// Build the node
		uint32_t build(const std::vector<const Triangle*> &triangles, uint32_t depth);
		// Intersect the node
		bool intersect(const Ray &ray, Intersection *intersection) const;

		BoundingBox bbox;
		std::vector<const Triangle*> triangles;
		std::array<Node*, 2> childrens;
	};
public:
	MeshBVH(Material *material);
	MeshBVH(const MeshBVH &) = delete;
	MeshBVH &operator=(const MeshBVH &) = delete;
	// Build the BVH
	void build() override;
	// Intersect the BVH
	bool intersect(const Ray &ray, Intersection *intersection) const override;

	const Node &getRoot() const { return m_rootNode; }
private:
	Node m_rootNode;
};

}