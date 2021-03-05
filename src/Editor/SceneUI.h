#pragma once

#include "EditorUI.h"

#include "../Prim/Core/Scene.h"

namespace viewer {

enum class CameraType {
	Perspective,
	Orthographic,
	Environment,
	Undefined
};

enum class HitableType {
	Mesh,
	MeshBVH,
	MeshOctree,
	Sphere,
	Triangle,
	Transform,
	Undefined
};
enum class MaterialType {
	Matte,
	Metal,
	Glass,
	Undefined
};
enum class TextureType {
	Constant,
	Checker,
	Image,
	Undefined
};

struct SceneUI : public EditorWidget
{
public:
	void initialize() override;
	void destroy() override;
	void update(aka::Time::Unit deltaTime) override;
	void draw() override;

	void setScene(prim::Scene* scene) { m_scene = scene; }
	void setCamera(prim::Camera* camera) { m_camera = camera; }
	void setSampleCount(uint32_t spp) { m_spp = spp; }
private:
	uint32_t m_spp = 0;
	prim::Scene* m_scene = nullptr;
	prim::Camera* m_camera = nullptr;
	prim::Hitable* m_selectedHitable = nullptr;
};

};