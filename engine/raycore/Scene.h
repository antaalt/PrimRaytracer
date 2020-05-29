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

namespace raycore {

struct NodeBVH : Hitable {
	// bbox bounding volume hierarchy
};

struct Scene {
	void build();
	bool intersect(const Ray &ray, ComputedIntersection *info) const;

	std::vector<Hitable*> hitables;
	std::vector<Material*> materials;
	std::vector<Texture<float>*> textures;
	BoundingBox bbox;
};

}


