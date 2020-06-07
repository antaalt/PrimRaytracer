#include "MeshBVH.h"

#include "Random.h"
#include "MeshOctree.h"

namespace prim {


static const uint32_t maxChildHitable = 10; // max element in child node
static const uint32_t childCount = 2;
static const uint32_t maxKmeanDepth = 15;
static const float kmeanEpsilon = 0.001f;

MeshBVH::MeshBVH(Material *material) :
	Mesh(material),
	m_rootNode()
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

bool MeshBVH::intersect(const Ray & ray, Intersection *intersection) const
{
	if (!m_bbox.intersect(ray))
		return false;
	return m_rootNode.intersect(ray, intersection);
}

MeshBVH::Node::Node() : 
	bbox(), 
	triangles(), 
	childrens{ nullptr }
{
}

MeshBVH::Node::~Node()
{
	for (uint32_t i = 0; i < childCount; i++)
		delete childrens[i];
}

uint32_t MeshBVH::Node::build(const std::vector<const Triangle*> &triangles, uint32_t depth)
{
	// 0. Compute bbox
	for (uint32_t iTri = 0; iTri < triangles.size(); iTri++)
	{
		bbox.include(triangles[iTri]->A.position);
		bbox.include(triangles[iTri]->B.position);
		bbox.include(triangles[iTri]->C.position);
	}

	// 1. check the depth and add element
	if (--depth == 0 || triangles.size() < maxChildHitable)
	{
		for (uint32_t iTri = 0; iTri < triangles.size(); iTri++)
			this->triangles.push_back(triangles[iTri]);
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
			triangles[triIndex[0]]->center(),
			triangles[triIndex[1]]->center()
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
				const geometry::point3f triBarycentre = triangle->center();
				ASSERT(bbox.contain(triBarycentre), "Should be inside");
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
					float weight = triangle->area();
					point3f triBarycentre = triangle->center();
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
			this->childrens[iGroup] = new Node;
			nodeCount += this->childrens[iGroup]->build(subGroup[iGroup], depth);
			ASSERT(this->bbox.contain(this->childrens[iGroup]->bbox), "Should contain container");
			nodeCount++;
		}
		return nodeCount;
	}
}

bool MeshBVH::Node::intersect(const Ray & ray, Intersection *intersection) const
{
	if (!bbox.intersect(ray))
		return false;
	bool hit = false;
	if (isLeaf())
	{
		for (const Triangle *triangle : triangles)
		{
			if (triangle->intersect(ray, intersection))
			{
				hit = true;
			}
		}
		return hit;
	}
	else
	{
		for (uint32_t i = 0; i < childCount; i++)
		{
			if (childrens[i]->intersect(ray, intersection))
			{
				hit = true;
			}
		}
		return hit;
	}
}

}