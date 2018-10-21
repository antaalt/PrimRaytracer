#pragma once
#include <string>
#include "Scene.h"

namespace raycore {

	namespace GLTF {
		bool load(std::string path, Scene &scene);
		bool write(std::string path, const Scene &scene);
	}

	namespace OBJ {
		struct Face {
			Triangle posID;
			Triangle normID;
			Triangle uvID;
		};
		bool load(std::string path, Mesh &scene);
		bool write(std::string path, const Mesh &scene);
	}

	namespace builder {
		bool build(Scene &scene);
	}
}


