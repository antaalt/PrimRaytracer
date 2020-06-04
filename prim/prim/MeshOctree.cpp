#include "MeshOctree.h"


namespace prim {

static const uint32_t maxOctreeDepth = 6;
static const uint32_t octreeChildCount = 8;
static const uint32_t maxChildHitable = 10;

MeshOctree::MeshOctree(const mat4f &transform, Material *material) :
	Mesh(transform, material),
	m_rootNode(this)
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

bool MeshOctree::intersect(const Ray & ray, Intersection &intersection) const
{
	if (!m_bbox.intersect(ray))
		return false;
	return m_rootNode.intersect(ray, intersection);
}

MeshOctree::Node::Node(MeshOctree * mesh) :
	m_mesh(mesh),
	m_bbox(),
	m_childrens{ nullptr },
	m_origin(0.f)
{
}

MeshOctree::Node::~Node()
{
	for (unsigned int i = 0; i < octreeChildCount; i++)
		delete m_childrens[i];
}

uint32_t MeshOctree::Node::build(const std::vector<const Triangle*> &triangles, uint32_t depth)
{
	// Compute bbox
	for (uint32_t iTri = 0; iTri < triangles.size(); iTri++)
	{
		m_bbox.include(m_mesh->m_positions[triangles[iTri]->A]);
		m_bbox.include(m_mesh->m_positions[triangles[iTri]->B]);
		m_bbox.include(m_mesh->m_positions[triangles[iTri]->C]);
	}

	if (--depth == 0 || triangles.size() <= maxChildHitable)
	{
		for (uint32_t iTri = 0; iTri < triangles.size(); iTri++)
			m_triangles.push_back(triangles[iTri]);
		ASSERT(isLeaf(), "Should be a leaf");
		return 0;
	}
	// Get optimal center of bbox
#if 0
	point3f c(0.f);
	for (uint32_t iTri = 0; iTri < triangles.size(); iTri++)
		c = c + (m_mesh->m_positions[triangles[iTri].A] + m_mesh->m_positions[triangles[iTri].B] + m_mesh->m_positions[triangles[iTri].C]) / 3.f;
	c = c / static_cast<float>(triangles.size());
	ASSERT(m_bbox.contain(c), "Not inside");
#else
	m_origin = m_bbox.center();
#endif
	unsigned int created = 0;
	for (int iChild = 0; iChild < octreeChildCount; iChild++)
	{
		// Compute new bounding box for this child
		BoundingBox childBox;
		childBox.min.x = (iChild & (1 << 2)) ? m_origin.x : m_bbox.min.x;
		childBox.max.x = (iChild & (1 << 2)) ? m_bbox.max.x : m_origin.x;
		childBox.min.y = (iChild & (1 << 1)) ? m_origin.y : m_bbox.min.y;
		childBox.max.y = (iChild & (1 << 1)) ? m_bbox.max.y : m_origin.y;
		childBox.min.z = (iChild & (1 << 0)) ? m_origin.z : m_bbox.min.z;
		childBox.max.z = (iChild & (1 << 0)) ? m_bbox.max.z : m_origin.z;
		ASSERT(m_bbox.contain(childBox), "Not inside");

		std::vector<const Triangle*> childTri;
		for (uint32_t iTri = 0; iTri < triangles.size(); iTri++)
		{
			const Triangle *tri = triangles[iTri];
			// TODO fix when all point out, but part of triangle inside child
			if (childBox.contain(m_mesh->m_positions[tri->A]) || childBox.contain(m_mesh->m_positions[tri->B]) || childBox.contain(m_mesh->m_positions[tri->C]))
				childTri.push_back(tri);
		}
		if (childTri.size() > 0)
		{
			m_childrens[iChild] = new Node(m_mesh);
			created += m_childrens[iChild]->build(childTri, depth);
			created++;
		}
	}
	return created;
}

bool MeshOctree::Node::intersect(const Ray & ray, Intersection &intersection) const
{
	if (!m_bbox.intersect(ray))
		return false;
	if (isLeaf())
	{
		bool terminateOnFirstHit = intersection.terminateOnFirstHit();
		for (const Triangle *triangle : m_triangles)
			if (m_mesh->intersectTri(*triangle, ray, intersection) && terminateOnFirstHit)
				return true;
		return intersection.valid();
	}
	else
	{
		bool terminateOnFirstHit = intersection.terminateOnFirstHit();
		for (uint32_t i = 0; i < octreeChildCount; i++)
			if (m_childrens[i]->intersect(ray, intersection) && terminateOnFirstHit)
				return true;
		return intersection.valid();
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