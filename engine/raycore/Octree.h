#pragma once
#include "Accelerator.h"
#include "BoundingBox.h"
#include "Triangle.h"
#include "Material.h"

#define MAX_OCTREE_DEPTH 6	// Max depth of the octree
#define MAX_OCTREE_CHILD 10	// Max number of child to stop the octree
#define OCTREE_CHILD_NODE 8

/**
 *	Octree class for triangles only.
 *	TODO list :
 *	- make other types compatibles (sphere...)
 **/

namespace raycore {
namespace prim {

struct TriangleBounded : prim::Triangle, prim::BoundingBox {
	TriangleBounded(const prim::Triangle &tri) : Triangle(tri.A, tri.B, tri.C)
	{
		this->material = tri.material;
		this->include(tri.A.position);
		this->include(tri.B.position);
		this->include(tri.C.position);
	}
};

class OctNode : public BoundingBox {
public:
	OctNode();
	OctNode(const point3f &min, const point3f &max);
	~OctNode();

	// Check if the ray hit a triangle from the Octree
	bool intersect(const tracer::Ray &ray, Intersection *intersection) const;
			
	// Get the index of the sub-bbox the point is in
	unsigned int getOctant(const vec3f &point) const;

	// Is the node a leaf ? (has it child ?)
	bool isLeafNode() const;

	// Init the acceleration structure
	unsigned int init(std::vector<const TriangleBounded*> &triangles, unsigned int depth = 0);

private:
	std::vector<const TriangleBounded*> triangles;	// List of triangles of the node
	OctNode* childrens[8];							// Childrens of the node
	point3f origin;									// Center of the octree separating all childs
};

// @see https://stackoverflow.com/questions/24711525/octree-implementation-for-triangular-mesh-and-particles
// @see http://www.flipcode.com/harmless/issue02.htm#octrees
class Octree : public Accelerator
{
public:
	Octree(const std::vector<Hitable*> &prim);
	~Octree();

	virtual bool intersect(const tracer::Ray &ray, Intersection *intersection) const;
	virtual bool intersect(const tracer::Ray &ray) const;
private:
	OctNode *root; // Root node of the octree
	std::vector<TriangleBounded> triangleBounded;
};

}
}
