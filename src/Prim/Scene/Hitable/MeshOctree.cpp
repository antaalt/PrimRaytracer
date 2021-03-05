#include "MeshOctree.h"

#include <Aka/Core/Debug.h>

namespace prim {

static const uint32_t maxOctreeDepth = 6;
static const uint32_t octreeChildCount = 8;
static const uint32_t maxChildHitable = 10;

MeshOctree::MeshOctree(Material *material) :
	Mesh(material),
	m_rootNode()
{
}

void MeshOctree::build()
{
	Mesh::build();
	std::vector<const Triangle*> triangles;
	for (Triangle &triangle : m_triangles)
		triangles.push_back(&triangle);
	m_rootNode.build(triangles, maxOctreeDepth);
}

bool MeshOctree::intersect(const Ray & ray, Intersection *intersection) const
{
	if (!m_bbox.intersect(ray))
		return false;
	return m_rootNode.intersect(ray, intersection);
}

MeshOctree::Node::Node() :
	bbox(),
	childrens{ nullptr },
	origin(0.f)
{
}

MeshOctree::Node::~Node()
{
	for (unsigned int i = 0; i < octreeChildCount; i++)
		delete childrens[i];
}

uint32_t MeshOctree::Node::build(const std::vector<const Triangle*> &triangles, uint32_t depth)
{
	// Compute bbox
	for (uint32_t iTri = 0; iTri < triangles.size(); iTri++)
		triangles[iTri]->include(bbox);

	if (--depth == 0 || triangles.size() <= maxChildHitable)
	{
		for (uint32_t iTri = 0; iTri < triangles.size(); iTri++)
			this->triangles.push_back(triangles[iTri]);
		ASSERT(isLeaf(), "Should be a leaf");
		return 0;
	}
	// Get optimal center of bbox
#if 1
	point3f c(0.f);
	for (uint32_t iTri = 0; iTri < triangles.size(); iTri++)
		c += vec3f(triangles[iTri]->center());
	c = c / static_cast<float>(triangles.size());
	ASSERT(bbox.contain(c), "Not inside");
#else
	origin = bbox.center();
#endif
	unsigned int created = 0;
	for (int iChild = 0; iChild < octreeChildCount; iChild++)
	{
		// Compute new bounding box for this child
		BoundingBox childBox;
		childBox.min.x = (iChild & (1 << 2)) ? origin.x : bbox.min.x;
		childBox.max.x = (iChild & (1 << 2)) ? bbox.max.x : origin.x;
		childBox.min.y = (iChild & (1 << 1)) ? origin.y : bbox.min.y;
		childBox.max.y = (iChild & (1 << 1)) ? bbox.max.y : origin.y;
		childBox.min.z = (iChild & (1 << 0)) ? origin.z : bbox.min.z;
		childBox.max.z = (iChild & (1 << 0)) ? bbox.max.z : origin.z;
		ASSERT(bbox.contain(childBox), "Not inside");

		std::vector<const Triangle*> childTri;
		for (uint32_t iTri = 0; iTri < triangles.size(); iTri++)
		{
			const Triangle *tri = triangles[iTri];
			// TODO fix when all point out, but part of triangle inside child
			if (childBox.contain(tri->A.position) || childBox.contain(tri->B.position) || childBox.contain(tri->C.position))
				childTri.push_back(tri);
		}
		if (childTri.size() > 0)
		{
			childrens[iChild] = new Node;
			created += childrens[iChild]->build(childTri, depth);
			created++;
		}
	}
	return created;
}

bool MeshOctree::Node::intersect(const Ray & ray, Intersection *intersection) const
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
		for (uint32_t i = 0; i < octreeChildCount; i++)
		{
			if (childrens[i]->intersect(ray, intersection))
			{
				hit = true;
			}
		}
		return hit;
	}
}


/*unsigned int getOctant(const vec3f & point)
{
	unsigned int oct = 0;
	if (point.x >= origin.x) oct |= (1 << 2);
	if (point.y >= origin.y) oct |= (1 << 1);
	if (point.z >= origin.z) oct |= (1 << 0);
	return oct;
}*/

}