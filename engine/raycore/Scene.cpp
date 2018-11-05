#include "Scene.h"
#include "Triangle.h"
#include "Sphere.h"
#include "BVH.h"
#include "Octree.h"
#include "NoAccel.h"

namespace raycore {
	namespace prim {

		Node::Node() : accelerator(nullptr), transform(mat4::identity())
		{
		}
		Node::~Node()
		{
			if (accelerator != nullptr)
				delete accelerator;
		}
		Group::Group()
		{
		}
		Group::~Group()
		{
			for (ChildBound *bound : bounds)
				delete bound;
		}
		Node * Group::addChild(Node * node)
		{
			bounds.push_back(new ChildBound(node));
			return node;
		}
		bool Group::build()
		{
			std::vector<Hitable*> hitables;
			for (ChildBound *bound : bounds)
			{
				hitables.push_back(bound);
				bound->node()->build();
			}
			switch (accel)
			{
			case Acceleration::BVH:
				accelerator = new BVH(hitables);
				break;
			case Acceleration::OCTREE:
				accelerator = new Octree(hitables);
				break;
			case Acceleration::NONE:
				accelerator = new NoAccel(hitables);
				break;
			}
			return true;
		}
		Group::ChildBound::ChildBound(Node * node)
		{
			bbox = node->bounds();
			child = node;
		}
		Group::ChildBound::~ChildBound()
		{
			delete child;
		}
		bool Group::ChildBound::intersect(const tracer::Ray & ray, Intersection * intersection) const
		{
			if (!bbox.intersectBounds(ray))
				return false;
			return child->traverse(ray, intersection);
		}
		HitInfo Group::ChildBound::computeIntersection(const tracer::Ray & ray, const Intersection *intersection) const
		{
			ASSERT(false, "Should never be called");
			return HitInfo();
		}
		BoundingBox Group::ChildBound::computeBoundingBox() const
		{
			return bbox;
		}
		float Group::ChildBound::area() const
		{
			vec3 extent(bbox.max - bbox.min);
			return (extent.x * extent.y + extent.x * extent.z + extent.y * extent.z) * 2.f;
		}
		Node * Group::ChildBound::node()
		{
			return child;
		}
		Geometry::Geometry(const std::vector<Hitable*> &hitables) : hitables(hitables)
		{
			for (Hitable* hitable : hitables)
				bbox.include(hitable->computeBoundingBox());
		}

		Geometry::~Geometry()
		{
			for (Hitable* hitable : hitables)
				delete hitable;
		}

		bool Geometry::build()
		{
			switch (accel)
			{
			case Acceleration::BVH:
				accelerator = new BVH(hitables);
				break;
			case Acceleration::OCTREE:
				accelerator = new Octree(hitables);
				break;
			case Acceleration::NONE:
				accelerator = new NoAccel(hitables);
				break;
			}
			return false;
		}


		Scene::Scene() : rootNode(nullptr), lightDistribution(nullptr)
		{
		}

		Scene::~Scene()
		{
			if(rootNode != nullptr)
				delete rootNode;
			if (lightDistribution != nullptr)
				delete lightDistribution;
			for (Material *material : materials)
				delete material;
			for (Texture<colorHDR> *texture : textures)
				delete texture;
		}

		bool Scene::intersect(const tracer::Ray & ray, HitInfo & info) const
		{
			Intersection intersection;
			if (!rootNode->traverse(ray, &intersection))
				return false;
			info = intersection.compute(ray);
			return intersection.hit();
		}
		bool Scene::isOccluded(const tracer::Ray & ray) const
		{
			HitInfo info;
			return intersect(ray, info);
		}

		Material * Scene::addMaterial(Material * material)
		{
			materials.push_back(material);
			return materials.back();
		}
		Texture<colorHDR> * Scene::addTexture(Texture<colorHDR>* texture)
		{
			textures.push_back(texture);
			return textures.back();
		}

		Group* Scene::setRoot(Group* root)
		{
			rootNode = root;
			return root;
		}
		void Scene::setLightDistribution(LightDistribution * lightDistribution)
		{
			this->lightDistribution = lightDistribution;
		}
		const LightDistribution * Scene::getLightDistribution() const
		{
			return lightDistribution;
		}
		bool Scene::build()
		{
			return rootNode->build();
		}
	}
}