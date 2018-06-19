#include "Scene.h"


#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tiny_gltf.h"

namespace Application {

	Scene::Scene()
	{
	}


	Scene::~Scene()
	{
	}

	bool Scene::loadScene(std::string path, FileFormat fileformat)
	{
		switch (fileformat)
		{
		default:
		case FileFormat::NONE:
			// TODO use assimp
			GL::Program *glProgram = new GL::Program();
			bool res = glProgram->attachShader(GL::Shader::VERTEX_SHADER, "../shaders/simpleShader.vert");
			ASSERT(res == true, "vertexShaderAttach failed");
			res = glProgram->attachShader(GL::Shader::FRAGMENT_SHADER, "../shaders/simpleShader.frag");
			ASSERT(res == true, "fragmentShaderAttach failed");
			res = glProgram->linkProgram();
			ASSERT(res == true, "LinkProgram failed");

			// TODO load a file
			Node &root = addNode();
			m_roots.push_back(&root);

			Mesh &mesh = addMesh();
			root.setMesh(&mesh);
			// Create a cube
			for (int x = 0; x < 2; x++)
			{
				for (int y = 0; y < 2; y++)
				{
					for (int z = 0; z < 2; z++)
					{
						Vertex vert;
						vert.position = glm::vec3(x, y, z);
						vert.normal = glm::normalize(glm::vec3(x, y, z));
						vert.color = glm::vec4(x, y, z, 1.f);
						vert.texCoord[0] = glm::vec2(x, y);
						mesh.addVertex(vert);
					}
				}
			}
			mesh.addFace(Face(0, 1, 2));
			mesh.addFace(Face(2, 1, 3));
			mesh.addFace(Face(2, 7, 3));
			mesh.addFace(Face(6, 7, 2));
			mesh.addFace(Face(3, 5, 1));
			mesh.addFace(Face(7, 5, 3));
			mesh.addFace(Face(0, 5, 1));
			mesh.addFace(Face(5, 4, 0));
			mesh.addFace(Face(6, 4, 2));
			mesh.addFace(Face(2, 4, 0));
			mesh.addFace(Face(6, 4, 7));
			mesh.addFace(Face(7, 4, 5));

			res = mesh.createVAO();
			ASSERT(res == true, "VAO failed creation");

			Material &material = addMaterial();
			material.setProgram(glProgram);
			Image image;
			res = image.loadFromFile("../textures/ocean.jpg");
			ASSERT(res == true, "Image not loaded");
			GL::Texture32 *texture = new GL::Texture32(image.data(), image.width(), image.height(), static_cast<GL::Depth>(image.stride()));
			// LEAK HERE
			material.setTexture(TextureType::COLOR_TEXTURE, texture);
			mesh.setMaterial(&material);
			Camera &camera = addCamera();
			camera.setLocalTransform(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 1.f)));
			m_currentCamera = 0;
			break;
		case FileFormat::GLTF:
			Loader loader(*this);
			return loader.loadFromGLTF(path);
			break;
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
		for(int iRoot = 0; iRoot < m_roots.size(); iRoot++)
			m_roots[iRoot]->draw(glm::mat4(1.f), camera->getView(), camera->getProjection());
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

	Mesh & Scene::addMesh()
	{
		m_meshes.emplace_back();
		Mesh &mesh = m_meshes.back();
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

	/*Node * Scene::getNode(size_t index)
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
	}*/

	Scene::Loader::Loader(Scene &scene) : m_scene(scene)
	{
	}

	Scene::Loader::~Loader()
	{
	}

	bool Scene::Loader::loadFromGLTF(std::string path)
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

		for (size_t iMesh = 0; iMesh < model.meshes.size(); iMesh++)
		{
			tinygltf::Mesh &mesh = model.meshes[iMesh];
			for (size_t iPrimitive = 0; iPrimitive < mesh.primitives.size(); iPrimitive++)
			{
				tinygltf::Primitive &primitive = mesh.primitives[iPrimitive];
				for (auto it = primitive.attributes.begin(); it != primitive.attributes.end(); it++)
				{

				}
			}
		}
		for (size_t iNode = 0; iNode < model.nodes.size(); iNode++)
		{
			tinygltf::Node &node = model.nodes[iNode];
			Node &newNode = m_scene.addNode();
			glm::mat4 localTransform;
			if (node.matrix.size() == 16)
			{
				for (int col = 0; col < 4; col++)
					for (int row = 0; row < 4; row++)
						localTransform[col][row] = static_cast<float>(node.matrix[col * 4 + row]);
			}
			else
			{
				glm::mat4 translation(1.f);
				glm::quat rotation(1.f, 0.f, 0.f, 0.f);
				glm::mat4 scale(1.f);

				if (node.translation.size() == 3)
					for (int i = 0; i < 3; i++)
						translation[3][i] = static_cast<float>(node.translation[i]);

				if (node.scale.size() == 3)
					for (int i = 0; i < 3; i++)
						scale[i][i] = static_cast<float>(node.scale[i]);

				if (node.rotation.size() == 4)
					for (int i = 0; i < 4; i++)
						rotation[i] = static_cast<float>(node.rotation[i]);

				localTransform = translation * glm::mat4_cast(rotation) * scale;
			}
			newNode.setLocalTransform(localTransform);
			for (int iChildNode = 0; node.children.size(); iChildNode++)
			{
				Node &childNode = m_scene.m_nodes[node.children[iChildNode]];
				newNode.addChild(&childNode);
			}
		}

		return true;
	}

}