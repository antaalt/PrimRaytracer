#pragma once


#include "../Config.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"
#include "tiny_gltf.h"
#include "../Texture.h"

namespace app {

	enum class FileFormat {
		NONE,
		GLTF
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		bool render();

		Node &addNode(Node *parent = nullptr);
		Mesh &addMesh();
		Camera &addCamera(Node *parent = nullptr);
		Light &addLight(Node *parent = nullptr);
		Material &addMaterial();
		GL::Texture32 &addTexture(const std::vector<unsigned char> &data, unsigned int width, unsigned int height, unsigned int components);

		Camera &getCurrentCamera();

		std::vector<GL::Texture32> textures;
		std::vector<Node> nodes;
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
		std::vector<Light> lights;
		std::vector<Camera> cameras;

		unsigned int currentCamera;

		std::vector<Node*> roots;

		struct GLTF {
			static bool load(std::string path, Scene &scene);
		};
		struct CUSTOM {
			static bool load(std::string path, Scene &scene);
		};
	};

}