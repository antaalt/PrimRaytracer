#pragma once
#include "Accelerator.h"
#include "Primitive.h"
#include "BoundingBox.h"


namespace app {
	namespace tracer {


		class OctNode : public prim::BoundingBox {
		public:
			Primitive *mesh;
			Matrix4 transform;

			void addChildren();

		private:
			OctNode* parent;
			OctNode* childrens[8];
		};


		class Octree : public Accelerator
		{
		public:
			Octree();
			~Octree();

			virtual bool build(const Scene &scene);

			virtual bool intersect(const Ray &ray, prim::HitInfo &info) const;

			virtual prim::HitInfo computeIntersection(const tracer::Ray &ray, const prim::Intersection &intersection) const;

			virtual bool add(const Primitive &primitive);

		private:
			OctNode *m_root;
			OctNode *m_childrens[8];
			Point3 m_origin;			// Center of the tree
			Vector3 m_halfDimension;	// Dimension of the tree
		};

	}
}
