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

struct NodeBVH : Hitable {
	// bbox bounding volume hierarchy with transform
};

struct Scene {
	void build();
	bool intersect(const Ray &ray, Intersection &intersection) const;

	std::vector<Light*> lights;
	std::vector<Hitable*> hitables;
	std::vector<Material*> materials;
	std::vector<Texture4f*> textures;
	BoundingBox bbox;
};

}


