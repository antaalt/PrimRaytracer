#include "Octree.h"
#include "Material.h"
#include <map>
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
			// TODO move default build to accelerator constructor and call it from here
			std::map<const Texture*, Texture*> mapTexture;
			//std::map<unsigned int, const prim::Material*> mapMaterials; // TODO implement map
			this->textures.reserve(scene.textures.size());
			for (size_t iTex = 0; iTex < scene.textures.size(); iTex++)
			{
				const Texture &texture = scene.textures[iTex];
				this->textures.push_back(texture);
				Texture &tex = this->textures.back();
				mapTexture.insert(std::make_pair(&texture, &tex));
			}
			this->materials.reserve(scene.materials.size());
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
				auto it = mapTexture.find(material.texture);
				newMaterial->setTexture((it == mapTexture.end()) ? nullptr : it->second);
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
					const mat4 transform = node.getModel();
					const mat3 normalTransform = convert::toMat3(transform);
					float det = transform.det();
					unsigned int triVert1 = 1;
					unsigned int triVert2 = 2;
					if (det < 0.f) // mirror triangle
					{
						triVert1 = 2;
						triVert2 = 1;
					}
					for (size_t iPrim = 0; iPrim < mesh->primitives.size(); iPrim++)
					{
						const Primitive::Ptr prim = mesh->primitives[iPrim];
						const Material::Ptr mat = prim->material;
						for (size_t iTri = 0; iTri < prim->triangles.size(); iTri++)
						{
							const Triangle &tri = prim->triangles[iTri];
							prim::Vertex data[3];
							for (unsigned int iVert = 0; iVert < 3; iVert++)
							{
								Vertex &v = prim->vertices[tri.vertices[iVert]];
								vec3 p = v.position;
								norm3 n = v.normal;
								p = transform * p;
								n = normalTransform * n;
								data[iVert] = prim::Vertex(p, n, v.texcoord, v.color);
								bbox.include(p);
							}
							prim::Triangle *newTri = new prim::Triangle(
								data[0],
								data[triVert1],
								data[triVert2]
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
			// TODO pass hitable list to octree initialisation and build it from importance
			initOctree(DEFAULT_DEPTH);
			for (size_t iHitable = 0; iHitable < this->hitables.size(); iHitable++)
			{
				prim::Hitable *hitable = this->hitables[iHitable];
				// assume all are triangles;
				prim::Triangle *triangle = static_cast<prim::Triangle*>(hitable);
				addTriangle(triangle);
			}
			return true;
		}

		bool Octree::intersect(const Ray & ray, prim::HitInfo &info) const
		{
			prim::Intersection intersection;
			if (!root->intersect(ray, intersection))
				return false;
			info = intersection.compute(ray);
			return true;
		}

		bool Octree::intersect(const Ray & ray) const
		{
			// TODO
			return false;
		}


		void Octree::initOctree(unsigned int maxDepth)
		{
			this->root = new OctNode(bbox.min, bbox.max);
			this->root->init(maxDepth);
		}

		void Octree::addTriangle(const prim::Triangle * tri)
		{
			root->addTriangle(tri);
		}

		bool OctNode::isLeafNode() const
		{
			return this->childrens[0] == nullptr;
		}

		void computeCenter()
		{

		}

		void OctNode::init(unsigned int maxDepth)
		{
			if (maxDepth <= 0)
				return;
			for (int iChild = 0; iChild < 8; iChild++)
			{
				// Compute new bounding box for this child
				point3 c = center(); // TODO compute center from an average of all triangles
				point3 lmin, lmax;
				lmin.x = (iChild & 4) ? c.x : min.x;
				lmax.x = (iChild & 4) ? max.x : c.x;
				lmin.y = (iChild & 2) ? c.y : min.y;
				lmax.y = (iChild & 2) ? max.y : c.y;
				lmin.z = (iChild & 1) ? c.z : min.z;
				lmax.z = (iChild & 1) ? max.z : c.z;
				this->childrens[iChild] = new OctNode(lmin, lmax);
				this->childrens[iChild]->init(maxDepth - 1);
			}
		}

		OctNode::OctNode() : OctNode(point3(0.f), point3(0.f))
		{
		}

		OctNode::OctNode(const point3 & min, const point3 & max) : BoundingBox(min - point3(EPSILON), max + point3(EPSILON))
		{
			for (unsigned int i = 0; i < 8; i++)
				this->childrens[i] = nullptr;
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

		unsigned int OctNode::getOctant(const vec3 & point) const
		{
			unsigned int oct = 0;
			point3 origin = center();
			if (point.x >= origin.x) oct |= (1 << 2);
			if (point.y >= origin.y) oct |= (1 << 1);
			if (point.z >= origin.z) oct |= (1 << 0);
			return oct;
		}
	}
}