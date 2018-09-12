#pragma once
#include "Accelerator.h"
#include "BoundingBox.h"
#include "Triangle.h"
#include "Material.h"

#define EPSILON_OCTREE 0.01f	// Force overlap of bounding box to prevent float imprecision
#define MAX_OCTREE_DEPTH 6		// Max depth of the octree
#define MAX_OCTREE_CHILD 10	// Max number of child to stop the octree
#define OCTREE_CHILD_NODE 8

/**
 *	Octree class for triangles only.
 *	TODO list :
 *	- make other types compatibles (sphere...)
 *	- Fix imprecision of float 
 **/

namespace app {
	namespace tracer {


		class OctNode : public prim::BoundingBox {
		public:
			OctNode();
			OctNode(const point3 &min, const point3 &max);
			~OctNode();

			// Check if the ray hit a triangle from the Octree
			bool intersect(const tracer::Ray &ray, prim::Intersection &intersection) const;
			
			// Get the index of the sub-bbox the point is in
			unsigned int getOctant(const vec3 &point) const;

			// Is the node a leaf ? (has it child ?)
			bool isLeafNode() const;

			// Init the acceleration structure
			unsigned int init(std::vector<const prim::Triangle*> &triangles, unsigned int depth = 0);

		private:
			std::vector<const prim::Triangle*> triangles;	// List of triangles of the node (only if leaf node)
			OctNode* parent;								// Parent of the node
			OctNode* childrens[8];							// Childrens of the node
			point3 origin;									// Center of the octree separating all childs
		};


		class Octree : public Accelerator
		{
		public:
			Octree();
			~Octree();

			virtual bool build(const Scene &scene);

			virtual bool intersect(const Ray &ray, prim::HitInfo &info) const;
			virtual bool intersect(const Ray &ray) const;
		private:
			OctNode *root; // Root node of the octree
		};

	}
}
