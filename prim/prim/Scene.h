#pragma once

#include <vector>

#include "Config.h"

// Textures
#include "Texture.h"
#include "ImageTexture.h"
#include "ConstantTexture.h"
#include "CheckerTexture.h"
// Materials
#include "Matte.h"
#include "Metal.h"
#include "Glass.h"
// Hitables
#include "Sphere.h"
#include "Triangle.h"
#include "Mesh.h"
#include "MeshBVH.h"
#include "MeshOctree.h"
// Lights
#include "PointLight.h"
#include "SunLight.h"
#include "PointLight.h"
#include "ShapeLight.h"
// Camera
#include "Camera.h"

namespace prim {

class TransformNode : public Hitable {
public:
	TransformNode(const mat4f &transform, Hitable *hitable);
	~TransformNode();

	void build() override;
	bool intersect(const Ray &ray, Intersection *intersection) const override;
	void include(BoundingBox &boundingBox) const override;

	void setTransform(const Transform &transform);
	const Transform &getTransform() const;
	const Hitable *getHitable() const { return m_hitable; }
private:
	Transform m_localToWorld;
	Transform m_worldToLocal;
	Hitable *m_hitable;
};

struct Scene {
	void build();
	bool intersect(const Ray &ray, Intersection &intersection) const;

	std::vector<Light*> lights;
	std::vector<TransformNode*> nodes;
	std::vector<Material*> materials;
	std::vector<Texture4f*> textures;
	BoundingBox bbox;
};

}


