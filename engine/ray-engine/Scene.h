#pragma once

#include <vector>
#include "Material.h"
#include "Texture.h"
#include "Primitive.h"

namespace Application {
	namespace RayTracer {

		struct Scene {
			std::vector<Primitive> primitives;
			std::vector<Material> materials;
			std::vector<Texture> textures;

			struct GLTF
			{
				static Scene load(std::string path);
				static bool write(const Scene &scene);
			};
		};
	}
}


