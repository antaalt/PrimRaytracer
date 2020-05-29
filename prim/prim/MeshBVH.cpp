#include "MeshBVH.h"

#include "Random.h"
#include "MeshOctree.h"

namespace prim {


static const uint32_t maxChildHitable = 5; // max element in child node
static const uint32_t childCount = 2;
static const uint32_t maxKmeanDepth = 15;
static const float kmeanEpsilon = 0.001f;

MeshBVH::MeshBVH(const mat4f &transform, Material *material) :
	Mesh(transform, material),
	m_rootNode(this)
{
}

void MeshBVH::build()
{
	if (m_bbox.valid())
		return; // already built
	Mesh::build();
	std::vector<const Triangle*> triangles;
	for (Triangle &triangle : m_triangles)
		triangles.push_back(&triangle);
	m_rootNode.build(triangles, 20);
}

bool MeshBVH::intersect(const Ray & ray, Intersection * intersection) const
{
	if (!m_bbox.intersect(ray))
		return false;
	return m_rootNode.intersect(ray, intersection);
}

MeshBVH::Node::Node(MeshBVH * mesh) : 
	m_mesh(mesh), 
	m_bbox(), 
	m_triangles(), 
	m_childrens{ nullptr }
{
}

MeshBVH::Node::~Node()
{
	for (uint32_t i = 0; i < childCount; i++)
		delete m_childrens[i];
}

uint32_t MeshBVH::Node::build(const std::vector<const Triangle*> &triangles, uint32_t depth)
{
	// 0. Compute bbox
	for (uint32_t iTri = 0; iTri < triangles.size(); iTri++)
	{
		m_bbox.include(m_mesh->m_transform * m_mesh->m_positions[triangles[iTri]->A]);
		m_bbox.include(m_mesh->m_transform * m_mesh->m_positions[triangles[iTri]->B]);
		m_bbox.include(m_mesh->m_transform * m_mesh->m_positions[triangles[iTri]->C]);
	}

	// 1. check the depth and add element
	if (--depth == 0 || triangles.size() < maxChildHitable)
	{
		for (uint32_t iTri = 0; iTri < triangles.size(); iTri++)
		{
			m_triangles.push_back(triangles[iTri]);
			ASSERT(triangles[iTri]->A < m_mesh->m_positions.size(), "Incorrect tri id");
			ASSERT(triangles[iTri]->B < m_mesh->m_positions.size(), "Incorrect tri id");
			ASSERT(triangles[iTri]->C < m_mesh->m_positions.size(), "Incorrect tri id");
		}
		ASSERT(isLeaf(), "Should be a leaf");
		return 0;
	}
	else
	{
		// 2. set two distinct random points as centroid
		uint32_t triIndex[2];
		triIndex[0] = Rand::sample<uint32_t>(0, (uint32_t)triangles.size() - 1);
		do {
			triIndex[1] = Rand::sample<uint32_t>(0, (uint32_t)triangles.size() - 1);
		} while (triIndex[1] == triIndex[0]);
		std::array<geometry::point3f, childCount> centroid = {
			m_mesh->center(*triangles[triIndex[0]]),
			m_mesh->center(*triangles[triIndex[1]])
		};
		unsigned int loop = 0;
		// TODO Instead of copying, sort triangle in array
		std::array<std::vector<const Triangle*>, childCount> subGroup;
		float epsilon[childCount];
		do {
			subGroup[0].clear();
			subGroup[1].clear();
			// 3. group by closest
			for (uint32_t iTri = 0; iTri < triangles.size(); iTri++)
			{
				const Triangle *triangle = triangles[iTri];
				const geometry::point3f triBarycentre = m_mesh->center(*triangle);
				ASSERT(m_bbox.contain(triBarycentre), "Should be inside");
				const std::array<float, childCount> dist = {
					geometry::point3f::distance(triBarycentre, centroid[0]),
					geometry::point3f::distance(triBarycentre, centroid[1])
				};
				if (dist[0] > dist[1])
					subGroup[1].push_back(triangle);
				else if (dist[0] < dist[1])
					subGroup[0].push_back(triangle);
				else // equal, fill an empty node
				{
					if (subGroup[0].size() == 0)
						subGroup[0].push_back(triangle);
					else
						subGroup[1].push_back(triangle);
				}
			}
			// 4. set two points as center of groups
			for (uint32_t iGroup = 0; iGroup < childCount; iGroup++)
			{
				ASSERT(subGroup[iGroup].size() > 0, "Should not be empty");
				geometry::point3f tmp(0.f);
				float weights = 0.f;
				for (const Triangle* triangle : subGroup[iGroup])
				{
					float weight = m_mesh->area(*triangle);
					point3f triBarycentre = m_mesh->center(*triangle);
					tmp += triBarycentre * weight;
					weights += weight;
				}
				geometry::point3f newGroupCentroid = tmp / weights;
				// Check weights
				epsilon[iGroup] = geometry::point3f::distance(newGroupCentroid, centroid[iGroup]);
				centroid[iGroup] = newGroupCentroid;
			}
			// 5. redo from 3 until difference small enough
		} while ((epsilon[0] > kmeanEpsilon && epsilon[1] > kmeanEpsilon) || loop++ > maxKmeanDepth);

		// 6. compute bbox of groups and create childrens
		uint32_t nodeCount = 0;
		for (unsigned int iGroup = 0; iGroup < childCount; iGroup++)
		{
			ASSERT(subGroup[iGroup].size() > 0, "Should not be empty");
			m_childrens[iGroup] = new Node(m_mesh);
			nodeCount += m_childrens[iGroup]->build(subGroup[iGroup], depth);
			ASSERT(m_bbox.contain(m_childrens[iGroup]->m_bbox), "Should contain container");
			nodeCount++;
		}
		return nodeCount;
	}
}

bool MeshBVH::Node::intersect(const Ray & ray, Intersection * intersection) const
{
	if (!m_bbox.intersect(ray))
		return false;
	if (isLeaf())
	{
		for (const Triangle *triangle : m_triangles)
			m_mesh->intersectTri(*triangle, ray, intersection);
		return intersection->valid();
	}
	else
	{
		for (uint32_t i = 0; i < childCount; i++)
			m_childrens[i]->intersect(ray, intersection);
		return intersection->valid();
	}
}

}