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
		bool intersect(const Ray &ray, Intersection &intersection) const;

		const Node &getChild(uint32_t iChild) const { return *m_childrens[iChild]; }
		const BoundingBox &getBbox() const { return m_bbox; }
		const std::vector<const Triangle*> &getTri() const { return m_triangles; }
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
	bool intersect(const Ray &ray, Intersection &intersection) const override;

	const Node &getRoot() const { return m_rootNode; }
private:
	Node m_rootNode;
};

}