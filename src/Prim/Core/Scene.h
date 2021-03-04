#pragma once

#include <vector>

#include "Geometry.h"
#include "Texture.h"
#include "Material.h"
#include "Hitable.h"
#include "Light.h"
#include "Camera.h"

namespace prim {

struct Scene {
	Scene();
	~Scene();

	void build();
	bool intersect(const Ray &ray, Intersection &intersection) const;

	std::vector<Light*> lights;
	std::vector<Hitable*> nodes;
	std::vector<Material*> materials;
	std::vector<Texture4f*> textures;
	BoundingBox bbox;
};

}


