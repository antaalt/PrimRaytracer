#include "Scene.h"

#include <cmath>
#include <iomanip>
#include <chrono>
#include <thread>
#include <future>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tiny_gltf.h"

namespace World {

	Scene::Scene()
	{
	}


	Scene::~Scene()
	{
	}

	bool Scene::loadScene(std::string path)
	{
		// TODO use assimp
		Application::GLProgram *glProgram = new Application::GLProgram();
		glProgram->attachShader(Application::GLShader::VERTEX_SHADER, "../shaders/shader.vert");
		glProgram->attachShader(Application::GLShader::FRAGMENT_SHADER, "../shaders/shader.frag");
		glProgram->linkProgram();

		// TODO load a file
		Node &root = addNode();
		m_root = &root;
		Mesh &mesh = addMesh(&root);
		Material &material = addMaterial();
		material.setProgram(glProgram);
		mesh.setMaterial(&material);
		Camera &camera = addCamera();
		camera.setProjection(90.f, DEFAULT_WIDTH / DEFAULT_HEIGHT);
		camera.setLocalTransform(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 1.f)));
		m_currentCamera = 0;

		return true;
	}

	bool Scene::loadFromGLTF(std::string path)
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF tinygltfctx;
		std::string err;
		bool ret = tinygltfctx.LoadASCIIFromFile(&model, &err, path);
		if (!err.empty())
		{
			Log::error(err);
			return false;
		}
		if (!ret)
		{
			Log::error("Failed to parse glTF");
			return false;
		}
		return true;
	}

	bool Scene::render()
	{
		// TODO shaders
		Camera *camera = getCurrentCamera();
		if (camera == nullptr)
		{
			Log::error("No camera set");
			return false;
		}
		m_root->draw(glm::mat4(1.f), camera->getView(), camera->getProjection());
		return true;
	}

	Camera * Scene::getCurrentCamera()
	{
		if (m_cameras.size() <= m_currentCamera)
			return nullptr;
		return &m_cameras[m_currentCamera];
	}

	const Camera * Scene::getCurrentCamera() const
	{
		if (m_cameras.size() <= m_currentCamera)
			return nullptr;
		return &m_cameras[m_currentCamera];
	}

	Node & Scene::addNode(Node *parent)
	{
		m_nodes.emplace_back();
		Node &node = m_nodes.back();
		if(parent != nullptr)
			node.setParent(parent);
		return node;
	}

	Mesh & Scene::addMesh(Node * parent)
	{
		m_meshes.emplace_back();
		Mesh &mesh = m_meshes.back();
		if (parent != nullptr)
			mesh.setParent(parent);
		return mesh;
	}

	Camera & Scene::addCamera(Node * parent)
	{
		m_cameras.emplace_back();
		Camera &camera = m_cameras.back();
		if (parent != nullptr)
			camera.setParent(parent);
		return camera;
	}

	Light & Scene::addLight(Node * parent)
	{
		m_lights.emplace_back();
		Light &light = m_lights.back();
		if (parent != nullptr)
			light.setParent(parent);
		return light;
	}

	Material & Scene::addMaterial()
	{
		m_materials.emplace_back();
		Material &material = m_materials.back();
		return material;
	}

	Node * Scene::getNode(size_t index)
	{
		return &m_nodes[index];
	}

	const Node * Scene::getNode(size_t index) const
	{
		return &m_nodes[index];
	}

	Mesh * Scene::getMesh(size_t index)
	{
		return &m_meshes[index];
	}

	const Mesh * Scene::getMesh(size_t index) const
	{
		return &m_meshes[index];
	}

	Camera * Scene::getCamera(size_t index)
	{
		return &m_cameras[index];
	}

	const Camera * Scene::getCamera(size_t index) const
	{
		return &m_cameras[index];
	}

	Light * Scene::getLight(size_t index)
	{
		return &m_lights[index];
	}

	const Light * Scene::getLight(size_t index) const
	{
		return &m_lights[index];
	}

	Material * Scene::getMaterial(size_t index)
	{
		return &m_materials[index];
	}

	const Material * Scene::getMaterial(size_t index) const
	{
		return &m_materials[index];
	}

}