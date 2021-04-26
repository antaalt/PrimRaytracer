#include "SceneUI.h"

#include "../Prim/Core/Geometry.h"

#include "../Prim/Scene/Camera/OrthographicCamera.h"
#include "../Prim/Scene/Camera/PerspectiveCamera.h"

#include "../Prim/Scene/Texture/ConstantTexture.h"
#include "../Prim/Scene/Texture/ImageTexture.h"
#include "../Prim/Scene/Texture/CheckerTexture.h"

#include "../Prim/Scene/Hitable/Sphere.h"
#include "../Prim/Scene/Hitable/Mesh.h"
#include "../Prim/Scene/Hitable/MeshBVH.h"
#include "../Prim/Scene/Hitable/MeshOctree.h"
#include "../Prim/Scene/Hitable/TransformNode.h"

#include "../Prim/Scene/Material/Glass.h"
#include "../Prim/Scene/Material/Matte.h"
#include "../Prim/Scene/Material/Metal.h"

namespace viewer {

const char* getName(HitableType type)
{
	switch (type)
	{
	case HitableType::Transform: return "Transform";
	case HitableType::Mesh: return "Mesh";
	case HitableType::MeshOctree: return "MeshOctree";
	case HitableType::MeshBVH: return "MeshBVH";
	case HitableType::Sphere: return "Sphere";
	default: return "Unknown";
	}
}

CameraType getType(const prim::Camera& camera)
{
	if (nullptr != dynamic_cast<const prim::PerspectiveCamera*>(&camera))
		return CameraType::Perspective;
	else if (nullptr != dynamic_cast<const prim::OrthographicCamera*>(&camera))
		return CameraType::Orthographic;
	//else if (nullptr != dynamic_cast<const prim::EnvironmentCamera*>(&camera))
	//	return CameraType::Environment;
	return CameraType::Undefined;
}

HitableType getType(const prim::Hitable& hitable)
{
	if (nullptr != dynamic_cast<const prim::TransformNode*>(&hitable))
		return HitableType::Transform;
	else if (nullptr != dynamic_cast<const prim::MeshBVH*>(&hitable))
		return HitableType::MeshBVH;
	else if (nullptr != dynamic_cast<const prim::MeshOctree*>(&hitable))
		return HitableType::MeshOctree;
	else if (nullptr != dynamic_cast<const prim::Mesh*>(&hitable))
		return HitableType::Mesh;
	else if (nullptr != dynamic_cast<const prim::Sphere*>(&hitable))
		return HitableType::Sphere;
	else if (nullptr != dynamic_cast<const prim::Triangle*>(&hitable))
		return HitableType::Triangle;
	return HitableType::Undefined;
}

MaterialType getType(const prim::Material& material)
{
	if (nullptr != dynamic_cast<const prim::Matte*>(&material))
		return MaterialType::Matte;
	else if (nullptr != dynamic_cast<const prim::Metal*>(&material))
		return MaterialType::Metal;
	else if (nullptr != dynamic_cast<const prim::Glass*>(&material))
		return MaterialType::Glass;
	return MaterialType::Undefined;
}

TextureType getType(const prim::Texture4f& texture)
{
	if (nullptr != dynamic_cast<const prim::ImageTexture4f*>(&texture))
		return TextureType::Image;
	else if (nullptr != dynamic_cast<const prim::CheckerTexture4f*>(&texture))
		return TextureType::Checker;
	else if (nullptr != dynamic_cast<const prim::ConstantTexture4f*>(&texture))
		return TextureType::Constant;
	return TextureType::Undefined;
}

void SceneUI::initialize()
{
}

void SceneUI::destroy()
{
}

void displayNode(const prim::MeshBVH& mesh, const prim::MeshBVH::Node& node)
{
	using namespace aka;
	char buffer[256];
	snprintf(buffer, 256, "Root##%p", &node);
	if (ImGui::TreeNodeEx(buffer, ImGuiTreeNodeFlags_None))
	{
		point3f min = node.bbox.min;
		point3f max = node.bbox.max;
		ImGui::Text("min (%f, %f, %f)", min.x, min.y, min.z);
		ImGui::Text("max (%f, %f, %f)", max.x, max.y, max.z);
		ImGui::Text("extent : %f", node.bbox.extent().norm());
		if (node.isLeaf())
		{
			for (const prim::Triangle* tri : node.triangles)
			{
				snprintf(buffer, 256, "tri##%p", tri);
				if (ImGui::TreeNodeEx(buffer, ImGuiTreeNodeFlags_None))
				{
					for (int i = 0; i < 3; i++)
					{
						point3f pos = tri->vertices[i].position;
						norm3f norm = tri->vertices[i].normal;
						uv2f texcoord = tri->vertices[i].texcoord;
						ImGui::Text("pos  (%f, %f, %f)", pos.x, pos.y, pos.z);
						ImGui::Text("norm (%f, %f, %f)", norm.x, norm.y, norm.z);
						ImGui::Text("uv   (%f, %f)", texcoord.u, texcoord.v);
						ImGui::Separator();
					}
					ImGui::TreePop();
				}
			}
		}
		else
		{
			ImGui::Separator();
			displayNode(mesh, *node.childrens[0]);
			ImGui::Separator();
			displayNode(mesh, *node.childrens[1]);
		}
		ImGui::TreePop();
	}
};

bool displayHitable(prim::Hitable* hitable)
{
	bool needUpdate = false;
	HitableType type = getType(*hitable);
	switch (type)
	{
	case HitableType::Transform: {
		prim::TransformNode* transform = dynamic_cast<prim::TransformNode*>(hitable);
		prim::mat4f mat = transform->getTransform().getMatrix();
		bool matUpdated = false;
		matUpdated |= ImGui::InputFloat4("##modeltransformcol1", mat.cols[0].data);
		matUpdated |= ImGui::InputFloat4("##modeltransformcol2", mat.cols[1].data);
		matUpdated |= ImGui::InputFloat4("##modeltransformcol3", mat.cols[2].data);
		matUpdated |= ImGui::InputFloat4("##modeltransformcol4", mat.cols[3].data);
		if (matUpdated)
		{
			needUpdate = true;
			transform->setTransform(prim::Transform(mat));
		}
		prim::Hitable* child = transform->getHitable();
		char buffer[256];
		const char* name = getName(getType(*child));
		snprintf(buffer, 256, "%s##%p", name, hitable);
		if (ImGui::TreeNodeEx(buffer, ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_DefaultOpen))
		{
			displayHitable(transform->getHitable());
			ImGui::TreePop();
		}
		break;
	}
	case HitableType::Mesh: {
		const prim::Mesh* mesh = dynamic_cast<const prim::Mesh*>(hitable);
		char buffer[256];
		for (size_t i = 0; i < mesh->getTriangleCount(); i++)
		{
			const prim::Triangle& triangle = mesh->getTriangle(i);
			if (ImGui::TreeNodeEx("Triangles"))
			{
				for (int i = 0; i < 3; i++)
				{
					snprintf(buffer, 256, "Vertex %d", i);
					if (ImGui::TreeNodeEx("Vertex", ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_DefaultOpen))
					{
						prim::point3f pos = triangle.vertices[i].position;
						prim::norm3f norm = triangle.vertices[i].normal;
						prim::uv2f texcoord = triangle.vertices[i].texcoord;
						ImGui::Text("pos  (%f, %f, %f)", pos.x, pos.y, pos.z);
						ImGui::Text("norm (%f, %f, %f)", norm.x, norm.y, norm.z);
						ImGui::Text("uv   (%f, %f)", texcoord.u, texcoord.v);
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
		break;
	}
	case HitableType::MeshOctree: {
		const prim::MeshOctree* meshOctree = dynamic_cast<const prim::MeshOctree*>(hitable);
		ImGui::Text("MeshOctree");
		break;
	}
	case HitableType::MeshBVH: {
		const prim::MeshBVH* meshBVH = dynamic_cast<const prim::MeshBVH*>(hitable);
		displayNode(*meshBVH, meshBVH->getRoot());
		break;
	}
	case HitableType::Sphere: {
		const prim::Sphere* sphere = dynamic_cast<const prim::Sphere*>(hitable);
		ImGui::Text("Radius : %f", sphere->getRadius());
		break;
	}
	case HitableType::Triangle: {
		const prim::Triangle* triangle = dynamic_cast<const prim::Triangle*>(hitable);
		char buffer[256];
		for (int i = 0; i < 3; i++)
		{
			snprintf(buffer, 256, "Vertex %d", i);
			if (ImGui::TreeNodeEx("Vertex", ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_DefaultOpen))
			{
				prim::point3f pos = triangle->vertices[i].position;
				prim::norm3f norm = triangle->vertices[i].normal;
				prim::uv2f texcoord = triangle->vertices[i].texcoord;
				ImGui::Text("pos  (%f, %f, %f)", pos.x, pos.y, pos.z);
				ImGui::Text("norm (%f, %f, %f)", norm.x, norm.y, norm.z);
				ImGui::Text("uv   (%f, %f)", texcoord.u, texcoord.v);
				ImGui::TreePop();
			}
		}
		break;
	}
	default:
		ImGui::Text("Invalid hitable");
		break;
	}
	return needUpdate;
}

bool displayMaterial(prim::Material* material)
{
	MaterialType type = getType(*material);
	switch (type)
	{
	case MaterialType::Matte:
		ImGui::Text("Matte");
		break;
	case MaterialType::Metal:
		ImGui::Text("Metal");
		break;
	case MaterialType::Glass:
		ImGui::Text("Glass");
		break;
	default:
		ImGui::Text("Invalid material");
		break;
	}
	return false;
}

bool displayTexture(prim::Texture4f* texture)
{
	TextureType type = getType(*texture);
	switch (type)
	{
	case TextureType::Image:
		ImGui::Text("Image");
		break;
	case TextureType::Checker:
		ImGui::Text("Checker");
		break;
	case TextureType::Constant:
		ImGui::Text("Constant");
		break;
	default:
		ImGui::Text("Invalid texture");
		break;
	}
	return false;
}

void SceneUI::update(aka::Time::Unit deltaTime)
{
	ImGuiIO& io = ImGui::GetIO();
	if (aka::Mouse::down(aka::MouseButton::ButtonLeft) && !io.WantCaptureMouse)
	{
		const aka::Position& p = aka::Mouse::position();
		prim::LinearRaySampler sampler;
		prim::RaySampler::Type sample = sampler(
			prim::vec2u(p.x, p.y), 
			prim::vec2u(aka::GraphicBackend::backbuffer()->width(), aka::GraphicBackend::backbuffer()->height())
		);
		prim::Ray ray = m_camera->generateRay(sample);
		prim::Intersection intersection;
		if (m_scene->intersect(ray, intersection))
		{
			m_selectedHitable = const_cast<prim::Hitable*>(intersection.hitable);
		}
	}
}

void SceneUI::draw()
{
	using namespace aka;
	bool needUpdate = false;
	if (ImGui::Begin("Prim"))
	{
		if (ImGui::CollapsingHeader("Settings"))
		{
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS) (%u spp)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate, m_spp);
			static int depth = 10;
			ImGui::SliderInt("Depth", &depth, 0, 15);
		}
		if (m_camera != nullptr && ImGui::CollapsingHeader("Camera"))
		{
			CameraType type = getType(*m_camera);
			if (type == CameraType::Perspective)
			{
				prim::PerspectiveCamera& pCamera = dynamic_cast<prim::PerspectiveCamera&>(*m_camera);
				static float zNear = 0.1f;
				static float zFar = 1000.f;
				bool projUpdated = false;
				projUpdated |= ImGui::SliderFloat("Field of view", &pCamera.hFov, 1.0f, 180.0f);
				projUpdated |= ImGui::SliderFloat("Focal distance", &pCamera.focalDistance, 0.f, 1000.f);
				projUpdated |= ImGui::SliderFloat("Lens radius", &pCamera.lensRadius, 0.f, 5.f);
				projUpdated |= ImGui::SliderFloat("Near", &zNear, 0.001f, zFar - 0.1f);
				projUpdated |= ImGui::SliderFloat("Far", &zFar, zNear + 0.1f, 1000.0f);
				if (projUpdated)
				{

					pCamera.perspective = mat4f::perspective(
						anglef::degree(pCamera.hFov),
						GraphicBackend::backbuffer()->width() / (float)GraphicBackend::backbuffer()->height(),
						zNear,
						zFar
					);
					pCamera.inverse = mat4f::inverse(pCamera.perspective);
					needUpdate = true;
				}
			}
			else if (type == CameraType::Orthographic)
			{
				prim::OrthographicCamera& pCamera = dynamic_cast<prim::OrthographicCamera&>(*m_camera);
			}

			ImGui::Text("Transform");
			mat4f mat = m_camera->transform.getMatrix();

			bool transformUpdated = false;
			transformUpdated |= ImGui::InputFloat4("##transformcol1", mat.cols[0].data);
			transformUpdated |= ImGui::InputFloat4("##transformcol2", mat.cols[1].data);
			transformUpdated |= ImGui::InputFloat4("##transformcol3", mat.cols[2].data);
			transformUpdated |= ImGui::InputFloat4("##transformcol4", mat.cols[3].data);

			if (transformUpdated)
			{
				m_camera->transform = prim::Transform(mat);
				needUpdate = true;
			}
			if (ImGui::Button("Identity"))
			{
				needUpdate = true;
				m_camera->transform = prim::Transform(geometry::mat4f::identity());
			}
		}
		if (m_scene != nullptr && ImGui::CollapsingHeader("Scene"))
		{
			if (ImGui::BeginChild("Hitables##section", ImVec2(0,200), true))
			{
				for (prim::Hitable* hitable : m_scene->nodes)
				{
					char buffer[256];
					HitableType type = getType(*hitable);
					const char* name = getName(type);
					snprintf(buffer, 256, "%s##%p", name, hitable);
					bool selected = (m_selectedHitable == hitable);
					if (ImGui::Selectable(buffer, &selected))
					{
						m_selectedHitable = hitable;
					}
				}
			}
			ImGui::EndChild();

			ImGui::Separator();

			if (m_selectedHitable != nullptr)
			{
				needUpdate |= displayHitable(m_selectedHitable);
				if (m_selectedHitable->getMaterial() != nullptr)
				{
					displayMaterial(m_selectedHitable->getMaterial());
					displayTexture(m_selectedHitable->getMaterial()->getTexture());
				}
			}

			/*if (ImGui::TreeNode("Materials", "Materials (%d)", m_scene->materials.size()))
			{
				for (prim::Material* material : m_scene->materials)
				{
					displayMaterial(material);
					ImGui::Separator();
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Textures", "Textures (%d)", m_scene->textures.size()))
			{
				for (prim::Texture4f* texture : m_scene->textures)
				{
					displayTexture(texture);
					ImGui::Separator();
				}
				ImGui::TreePop();
			}*/
		}

		ImGui::SameLine();
	}
	ImGui::End();
}

}