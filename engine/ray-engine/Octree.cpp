#include "Octree.h"
#include "Material.h"

namespace app {
	namespace tracer {

		Octree::Octree()
		{
		}


		Octree::~Octree()
		{
			for (size_t i = 0; i < this->hitables.size(); i++)
				delete this->hitables[i];
			for (size_t i = 0; i < this->materials.size(); i++)
				delete this->materials[i];
		}

		bool Octree::build(const Scene & scene)
		{
			for (size_t iMat = 0; iMat < scene.materials.size(); iMat++)
			{
				const Material &material = scene.materials[iMat];
				prim::Material::Ptr newMaterial;
				rand::Sampler sampler;
				switch (material.type)
				{
				case MaterialType::DIFFUSE:
					newMaterial = new prim::Diffuse(sampler);
					break;
				case MaterialType::SPECULAR:
					newMaterial = new prim::Specular();
					break;
				case MaterialType::DIELECTRIC:
					newMaterial = new prim::Dielectric(1.5f);
					break;
				case MaterialType::METAL:
					newMaterial = new prim::Metal(0.f, sampler);
					break;
				default:
					return false;
				}
				newMaterial->setColor(material.color);
				this->materials.push_back(newMaterial);
			}
			for (size_t iNode = 0; iNode < scene.nodes.size(); iNode++)
			{
				const Node &node = scene.nodes[iNode];
				const Shape *shape = node.shape;
				if (shape == nullptr)
					continue;
				switch (shape->type())
				{
				case ShapeType::MESH:
				{
					const Mesh *mesh = reinterpret_cast<const Mesh*>(shape);
					const Matrix4 transform = node.getModel();
					for (size_t iPrim = 0; iPrim < mesh->primitives.size(); iPrim++)
					{
						const Primitive::Ptr prim = mesh->primitives[iPrim];
						const Material::Ptr mat = prim->material;
						for (size_t iTri = 0; iTri < prim->triangles.size(); iTri++)
						{
							const Triangle &tri = prim->triangles[iTri];
							Vertex &vA = prim->vertices[tri.A];
							Vertex &vB = prim->vertices[tri.B];
							Vertex &vC = prim->vertices[tri.C];
							vA.position = transform * vA.position;
							vB.position = transform * vB.position;
							vC.position = transform * vC.position;
							bbox.include(vA.position);
							bbox.include(vB.position);
							bbox.include(vC.position);
							prim::Triangle *newTri = new prim::Triangle(
								prim::Vertex(vA.position, vA.normal, vA.texcoord, vA.color),
								prim::Vertex(vB.position, vB.normal, vB.texcoord, vB.color),
								prim::Vertex(vC.position, vC.normal, vC.texcoord, vC.color)
							);
							newTri->material = this->materials[prim->material->index];
							this->hitables.push_back(newTri);
						}
					}
				}
				break;
				case ShapeType::SPHERE:
				case ShapeType::PARALLELOGRAM:
				default:
					std::runtime_error("Type not supported");
				}

			}
			initOctree(1);
			for (size_t iHitable = 0; iHitable < this->hitables.size(); iHitable++)
			{
				prim::Hitable *hitable = this->hitables[iHitable];
				// assume all are triangles;
				prim::Triangle *triangle = static_cast<prim::Triangle*>(hitable);
				
			}
			return true;
		}

		bool Octree::intersect(const Ray & ray, prim::HitInfo &info) const
		{
			prim::Intersection intersection;
			bool intersect = false;
			prim::Hitable::Ptr hitObject = nullptr;
			for (size_t iChild = 0; iChild < 8; iChild++)
			{
				OctNode* node = this->childrens[iChild];
				prim::Intersection localIntersection;
				if (node->intersect(ray, localIntersection))
					intersection.isClosestThan(localIntersection);
			}
			if (!intersection.hit())
				return false;
			info = intersection.compute(ray);
			return true;
		}

		void Octree::initOctree(unsigned int maxDepth)
		{
			for (int i = 0; i < 8; ++i) {
				// Compute new bounding box for this child
				Point3 newOrigin = bbox.center();
				newOrigin.x += halfDimension.x * (i & 4 ? .5f : -.5f);
				newOrigin.y += halfDimension.y * (i & 2 ? .5f : -.5f);
				newOrigin.z += halfDimension.z * (i & 1 ? .5f : -.5f);
				this->childrens[i] = new OctNode(newOrigin, halfDimension*.5f);
			}
		}

		void Octree::addTriangle(const prim::Triangle * tri)
		{
			for (unsigned int i = 0; i < 8; i++)
				this->childrens[i]->addTriangle(tri);
		}

		bool OctNode::isLeafNode() const
		{
			return this->childrens[0] == nullptr;
		}

		OctNode::OctNode() : OctNode(Point3(0.f), Vector3(0.f))
		{
		}

		OctNode::OctNode(const Point3 & newOrigin, const Vector3 & halfDimension)
		{
			for (unsigned int i = 0; i < 8; i++)
				this->childrens[i] = nullptr;
			this->max = newOrigin + halfDimension;
			this->min = newOrigin - halfDimension;
		}

		OctNode::~OctNode()
		{
			for (unsigned int i = 0; i < 8; i++)
				delete childrens[i];
		}

		void OctNode::addTriangle(const prim::Triangle * triangle)
		{
			if (isLeafNode())
				triangles.push_back(triangle);
			else
			{
				unsigned int octA = getOctant(triangle->A.position);
				unsigned int octB = getOctant(triangle->B.position);
				unsigned int octC = getOctant(triangle->C.position);
				childrens[octA]->addTriangle(triangle);
				if(octA != octB)
					childrens[octB]->addTriangle(triangle);
				if (octA != octC)
					childrens[octC]->addTriangle(triangle);
			}
		}

		void OctNode::addTriangles(const prim::Triangle * triangle, unsigned int count)
		{
			for (unsigned int i = 0; i < count; i++)
				addTriangle(&triangle[i]);
		}

		bool OctNode::intersect(const tracer::Ray & ray, prim::Intersection & intersection) const
		{
			if (!intersectBounds(ray))
				return false;
			if (isLeafNode())
			{
				for (size_t iHitable = 0; iHitable < triangles.size(); iHitable++)
				{
					prim::Intersection localIntersection;
					if (triangles[iHitable]->intersect(ray, localIntersection))
						intersection.isClosestThan(localIntersection);
				}
				return intersection.hit();
			}
			else
			{
				for (unsigned int i = 0; i < 8; i++)
				{
					prim::Intersection localIntersection;
					if (childrens[i]->intersect(ray, localIntersection))
						intersection.isClosestThan(localIntersection);
				}
				return intersection.hit();
			}
			return false;
		}

		unsigned int OctNode::getOctant(const Vector3 & point) const
		{
			unsigned int oct = 0;
			if (point.x >= origin.x) oct |= (1 << 2);
			if (point.y >= origin.y) oct |= (1 << 1);
			if (point.z >= origin.z) oct |= (1 << 0);
			return oct;
		}
	}
}