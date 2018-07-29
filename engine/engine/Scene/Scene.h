#pragma once


#include "../Config.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"
#include "tiny_gltf.h"

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

		bool loadScene(std::string path, FileFormat fileformat);


	public:
		bool render();

		Camera * getCurrentCamera();
		const Camera * getCurrentCamera() const;

		Node &addNode(Node *parent = nullptr);
		Mesh &addMesh();
		Camera &addCamera(Node *parent = nullptr);
		Light &addLight(Node *parent = nullptr);
		Material &addMaterial();

		/*Node *getNode(size_t index);
		const Node *getNode(size_t index) const;
		Mesh *getMesh(size_t index);
		const Mesh *getMesh(size_t index) const;
		Camera *getCamera(size_t index);
		const Camera *getCamera(size_t index) const;
		Light *getLight(size_t index);
		const Light *getLight(size_t index) const;
		Material *getMaterial(size_t index);
		const Material *getMaterial(size_t index) const;*/

	private:
		// TODO Octree
		std::vector<Node> m_nodes;
		std::vector<Mesh> m_meshes;
		std::vector<Material> m_materials;
		std::vector<Light> m_lights;

		std::vector<Camera> m_cameras;

		unsigned int m_currentCamera;

		std::vector<Node*> m_roots;

		class Loader
		{
		public:
			Loader(Scene &scene);
			~Loader();

			bool loadFromGLTF(std::string path);

		private:
			Scene &m_scene;
		};

	};

}