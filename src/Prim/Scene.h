#pragma once

#include <vector>

#include "Geometry.h"

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
#include "TransformNode.h"
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

struct Scene {
	Scene();
	~Scene();

	void build();
	bool intersect(const Ray &ray, Intersection &intersection) const;

	std::vector<Light*> lights;
	std::vector<TransformNode*> nodes;
	std::vector<Material*> materials;
	std::vector<Texture4f*> textures;
	BoundingBox bbox;
};

}


