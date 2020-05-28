#pragma once

#include <vector>
#include "Config.h"
// Textures
#include "Texture.h"
#include "TextureMap.h"
#include "ConstantTexture.h"
#include "Checker.h"
// Materials
#include "Matte.h"
#include "Metal.h"
#include "Glass.h"
// Primitives
#include "Accelerator.h"
#include "LightDistribution.h"

namespace raycore {

	namespace prim {

		// --- Nodes
		struct Node {
		public:
			Node();
			~Node();
			virtual bool traverse(const tracer::Ray &ray, Intersection *intersection) const {
				tracer::Ray transformedRay;
				transformedRay.origin = transform * ray.origin;
				transformedRay.direction = transform * ray.direction;
				if (!bbox.intersectBounds(transformedRay))
					return false;
				return accelerator->intersect(transformedRay, intersection);
			}
			virtual bool build() = 0;
			virtual const BoundingBox &bounds() const { return bbox; };
			void setAcceleration(Acceleration accel) { this->accel = accel; };
			virtual void setTransform(const mat4 &transform) { this->transform = inverse(transform); }
		protected:
			BoundingBox bbox;
			Acceleration accel;
			Accelerator *accelerator;
			mat4 transform; // called during traverse to transform the ray before passing it to accelerator
		};

		struct Group : Node {
			Group();
			~Group();
			Node* addChild(Node* node);
			virtual bool build();
		private:
			struct ChildBound : Hitable {
			public:
				ChildBound(Node* node);
				~ChildBound();
				virtual bool intersect(const tracer::Ray &ray, Intersection *intersection) const;
				virtual HitInfo computeIntersection(const tracer::Ray &ray, const Intersection *intersection) const;
				virtual BoundingBox computeBoundingBox() const;
				virtual float area() const;
				Node* node();
			private:
				BoundingBox bbox;
				Node* child;
			};
			std::vector<ChildBound*> bounds;
		};

		struct Geometry : Node {
			Geometry(const std::vector<Hitable*> &hitables);
			~Geometry();
			virtual bool build();
		private:
			std::vector<Hitable*> hitables;
		};

		class LightDistribution;

		// --- Scene
		struct Scene {
		public:
			Scene();
			~Scene();
			// Scene traversal
			virtual bool intersect(const tracer::Ray &ray, HitInfo &info) const;
			virtual bool isOccluded(const tracer::Ray &ray) const;

			// Scene preparation
			Material *addMaterial(Material* material);
			Texture<colorHDR> *addTexture(Texture<colorHDR>* texture);
			Group* setRoot(Group* root);
			Group* getRoot() { return rootNode; }
			void setLightDistribution(LightDistribution *lightDistribution);
			const LightDistribution *getLightDistribution() const;
			bool build();
		private:
			Group *rootNode;
			LightDistribution *lightDistribution;
			std::vector<Material*> materials;
			std::vector<Texture<colorHDR>*> textures;
		};
	}
}


