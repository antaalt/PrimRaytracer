#pragma once


#include "../Config.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"

namespace World {

	class Scene
	{
	public:
		Scene();
		~Scene();

		bool loadScene(std::string path);
		bool loadFromGLTF(std::string path);

		bool render();

		Camera * getCurrentCamera();
		const Camera * getCurrentCamera() const;

		Node &addNode(Node *parent = nullptr);
		Mesh &addMesh(Node *parent = nullptr);
		Camera &addCamera(Node *parent = nullptr);
		Light &addLight(Node *parent = nullptr);
		Material &addMaterial();

		Node *getNode(size_t index);
		const Node *getNode(size_t index) const;
		Mesh *getMesh(size_t index);
		const Mesh *getMesh(size_t index) const;
		Camera *getCamera(size_t index);
		const Camera *getCamera(size_t index) const;
		Light *getLight(size_t index);
		const Light *getLight(size_t index) const;
		Material *getMaterial(size_t index);
		const Material *getMaterial(size_t index) const;


	private:
		// TODO Octree
		std::vector<Node> m_nodes;
		std::vector<Mesh> m_meshes;
		std::vector<Material> m_materials;
		std::vector<Light> m_lights;

		std::vector<Camera> m_cameras;

		unsigned int m_currentCamera;

		Node *m_root;

	};

}