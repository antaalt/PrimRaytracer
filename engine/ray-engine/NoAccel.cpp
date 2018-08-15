#include "NoAccel.h"

#include "Triangle.h"
#include "Sphere.h"
#include "Parallelogram.h"

#include <map>


namespace app {
	namespace tracer {
		NoAccel::NoAccel()
		{
		}
		NoAccel::~NoAccel()
		{
			for (size_t i = 0; i < m_hitable.size(); i++)
				delete m_hitable[i];
			for (size_t i = 0; i < m_materials.size(); i++)
				delete m_materials[i];
		}
		bool NoAccel::build(const Scene & scene)
		{
			// TODO move default build to accelerator build and call it from here
			std::map<const Texture*, Texture*> mapTexture;
			//std::map<unsigned int, const prim::Material*> mapMaterials; // TODO implement map
			m_textures.reserve(scene.textures.size());
			for (size_t iTex = 0; iTex < scene.textures.size(); iTex++)
			{
				const Texture &texture = scene.textures[iTex];
				m_textures.push_back(texture);
				mapTexture.insert(std::make_pair(&texture, &m_textures.back()));
			}
			m_materials.reserve(scene.materials.size());
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
				m_materials.push_back(newMaterial);
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
							newTri->material = m_materials[prim->material->index];
							m_hitable.push_back(newTri);
						}
					}
				}
				break;
				case ShapeType::SPHERE:
				{
					const Sphere *sphere = reinterpret_cast<const Sphere*>(shape);
					const mat4 transform = node.getModel();
					prim::Sphere::Ptr newSphere = new prim::Sphere(sphere->center, sphere->radius);
					newSphere->material = m_materials[sphere->material->index];
					m_hitable.push_back(newSphere);
					bbox.include(sphere->center + point3(sphere->radius));
					bbox.include(sphere->center + point3(-sphere->radius));
				}
				break;
				case ShapeType::PARALLELOGRAM:
				{
					/*const Parallelogram *plane = reinterpret_cast<const Parallelogram*>(shape);
					const Matrix4 transform = node.getModel();
					prim::Parallelogram::Ptr newSphere = new prim::Parallelogram(plane->point[0], plane->point[1], plane->point[2], plane->normal);
					newSphere->material = m_materials[plane->material->index];
					m_hitable.push_back(newSphere);
					bbox.include(plane->point[0]);
					bbox.include(plane->point[1]);
					bbox.include(plane->point[2]);*/
					// TODO add 4th point
				}
				break;
				default:
					std::runtime_error("No corresponding type");
				}
				
			}
			m_hitableCount = m_hitable.size();
			return true;
		}
		bool NoAccel::intersect(const Ray & ray, prim::HitInfo & info) const
		{
			if (!bbox.intersectBounds(ray))
				return false;
			prim::Intersection intersection;
			for (size_t iHitable = 0; iHitable < m_hitableCount; iHitable++)
			{
				prim::Intersection localIntersection;
				if (m_hitable[iHitable]->intersect(ray, localIntersection))
				{
					intersection.isClosestThan(localIntersection);
				}
			}
			if (!intersection.hit())
				return false;
			
			info = intersection.compute(ray);
			return true;
		}
		bool NoAccel::intersect(const Ray & ray) const
		{
			if (!bbox.intersectBounds(ray))
				return false;
			prim::Intersection intersection;
			for (size_t iHitable = 0; iHitable < m_hitableCount; iHitable++)
				if (m_hitable[iHitable]->intersect(ray, intersection))
					return true;
			return false;
		}
	}
}

