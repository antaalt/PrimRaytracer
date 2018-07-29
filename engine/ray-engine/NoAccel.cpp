#include "NoAccel.h"

#include "Triangle.h"
#include "Sphere.h"


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
				newMaterial->albedo = material.color;
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
							newTri->material = m_materials[prim->material->index];
							m_hitable.push_back(newTri);
						}
					}
				}
				break;
				case ShapeType::SPHERE:
				{
					const Sphere *sphere = reinterpret_cast<const Sphere*>(shape);
					const Matrix4 transform = node.getModel();
					prim::Sphere::Ptr newSphere = new prim::Sphere(sphere->center, sphere->radius);
					newSphere->material = m_materials[sphere->material->index];
					m_hitable.push_back(newSphere);
					bbox.include(sphere->center + Point3(sphere->radius));
					bbox.include(sphere->center + Point3(-sphere->radius));
				}
				break;
				case ShapeType::PLANE:
				{

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
			if (!bbox.intersect(ray))
				return false;
			float minDistance = INVALID_INTERSECTION;
			prim::Intersection intersection;
			prim::Hitable::Ptr hitObject = nullptr;
			for (size_t iHitable = 0; iHitable < m_hitableCount; iHitable++)
			{
				prim::Intersection localIntersection;
				if (m_hitable[iHitable]->intersect(ray, localIntersection))
				{
					if (localIntersection.distance < minDistance)
					{
						hitObject = m_hitable[iHitable];
						minDistance = localIntersection.distance;
						intersection = localIntersection;
					}
				}
			}
			if (hitObject == nullptr)
				return false;
			
			info = hitObject->computeIntersection(ray, intersection);
			return true;
		}
	}
}

