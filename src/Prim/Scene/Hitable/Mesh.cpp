#include "Mesh.h"

#include "Triangle.h"

namespace prim {

Mesh::Mesh(Material *material) :
	Hitable(material)
{
}

void Mesh::build()
{
	for (const Triangle &triangle : m_triangles)
		triangle.include(m_bbox);
}

bool Mesh::intersect(const Ray & ray, Intersection *intersection) const
{
	// World to local transform
	if (!m_bbox.intersect(ray))
		return false;
	bool hit = false;
	for (const Triangle &tri : m_triangles)
	{
		if (tri.intersect(ray, intersection))
		{
			hit = true;
		}
	}
	return hit;
}

void Mesh::include(BoundingBox &boundingBox) const
{
	boundingBox.include(m_bbox);
}

size_t Mesh::getTriangleCount() const
{
	return m_triangles.size();
}

Triangle& Mesh::getTriangle(size_t index)
{
	return m_triangles[index];
}

const Triangle& Mesh::getTriangle(size_t index) const
{
	return m_triangles[index];
}

}
