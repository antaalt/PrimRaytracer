#pragma once
#include "Accelerator.h"
#include "BoundingBox.h"
#include "Triangle.h"


namespace app {
	namespace tracer {


		class OctNode : public prim::BoundingBox {
		public:
			OctNode();
			OctNode(const BoundingBox &bbox);
			~OctNode();
			void addTriangle(const prim::Triangle *triangle);
			void addTriangles(const prim::Triangle *triangle, unsigned int count);

			bool intersect(const tracer::Ray &ray, prim::Intersection &intersection) const;

			unsigned int getOctant(const Vector3 &point) const;
			bool isLeafNode() const;

		private:
			std::vector<const prim::Triangle*> triangles;
			OctNode* parent;			// Parent of the node
			OctNode* childrens[8];		// Childrens of the node
			Point3 origin;			// Center of the tree
			Vector3 halfDimension;
		};


		class Octree : public Accelerator
		{
		public:
			Octree();
			Octree(unsigned int maxDepth);
			~Octree();

			

			virtual bool build(const Scene &scene);

			virtual bool intersect(const Ray &ray, prim::HitInfo &info) const;
		private:
			void initOctree(unsigned int maxDepth);

			void addTriangle(const prim::Triangle *tri);
		private:
			OctNode *m_childrens[8];	// Childrens of the octree
			prim::BoundingBox bbox;		// Bounding box of the whole octree
			std::vector<prim::Hitable::Ptr> m_hitables;
			std::vector<prim::Material::Ptr> m_materials;
			Point3 m_origin;			// Center of the tree
			Vector3 m_halfDimension;	// Dimension of the tree
			unsigned int m_maxDepth;	// Max depth of the Octree
		};

	}
}
