#pragma once
#include "Accelerator.h"
#include "BoundingBox.h"
#include "Triangle.h"
#include "Material.h"

#define EPSILON 0.01f
#define DEFAULT_DEPTH 4 // TODO generate depending on triangles count


namespace app {
	namespace tracer {


		class OctNode : public prim::BoundingBox {
		public:
			OctNode();
			OctNode(const point3 &min, const point3 &max);
			~OctNode();
			void addTriangle(const prim::Triangle *triangle);

			bool intersect(const tracer::Ray &ray, prim::Intersection &intersection) const;

			unsigned int getOctant(const vec3 &point) const;
			bool isLeafNode() const;

			void init(unsigned int maxDepth);

		private:
			std::vector<const prim::Triangle*> triangles;
			OctNode* parent;			// Parent of the node
			OctNode* childrens[8];		// Childrens of the node
		};


		class Octree : public Accelerator
		{
		public:
			Octree();
			Octree(unsigned int maxDepth);
			~Octree();

			virtual bool build(const Scene &scene);

			virtual bool intersect(const Ray &ray, prim::HitInfo &info) const;
			virtual bool intersect(const Ray &ray) const;
		private:
			void initOctree(unsigned int maxDepth);

			void addTriangle(const prim::Triangle *tri);
		private:
			OctNode *root;								// Childrens of the octree
			prim::BoundingBox bbox;						// Bounding box of the whole octree
			std::vector<prim::Hitable::Ptr> hitables;	// Hitable inside the octree
			std::vector<prim::Material::Ptr> materials;	// materials of the hitables
			std::vector<Texture> textures;			// Textures of the hitables
			unsigned int maxDepth;						// Max depth of the Octree
		};

	}
}
