#pragma once

#include <vector>
#include <array>

#include "Mesh.h"
#include "BoundingBox.h"

namespace prim {

class MeshBVH final : public Mesh {
public:
	struct Node {
		Node(MeshBVH *mesh);
		Node(const Node &) = delete;
		Node &operator=(const Node &) = delete;
		~Node();
		// Check if the node is a leaf
		bool isLeaf() const { return m_triangles.size() != 0; }
		// Build the node
		uint32_t build(const std::vector<const Triangle*> &triangles, uint32_t depth);
		// Intersect the node
		bool intersect(const Ray &ray, Intersection *intersection) const;
	private:
		MeshBVH *m_mesh;
		BoundingBox m_bbox;
		std::vector<const Triangle*> m_triangles;
		std::array<Node*, 2> m_childrens;
	};
public:
	MeshBVH(const mat4f &transform, Material *material);
	MeshBVH(const MeshBVH &) = delete;
	MeshBVH &operator=(const MeshBVH &) = delete;
	// Build the BVH
	void build() override;
	// Intersect the BVH
	bool intersect(const Ray &ray, Intersection *intersection) const override;
private:
	Node m_rootNode;
};

}