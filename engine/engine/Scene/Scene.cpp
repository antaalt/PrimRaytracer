#include "Scene.h"


#define TINYGLTF_IMPLEMENTATION
/*#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT*/
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tiny_gltf.h"

namespace app {

	Scene::Scene()
	{
	}


	Scene::~Scene()
	{
	}

	bool Scene::render()
	{
		// TODO shaders
		Camera &camera = getCurrentCamera();

		for(size_t iRoot = 0; iRoot < roots.size(); iRoot++)
			roots[iRoot]->draw(glm::mat4(1.f), camera.getView(), camera.getProjection());
		return true;
	}

	Node & Scene::addNode(Node *parent)
	{
		nodes.emplace_back();
		Node &node = nodes.back();
		if(parent != nullptr)
			node.setParent(parent);
		return node;
	}

	Mesh & Scene::addMesh()
	{
		meshes.emplace_back();
		Mesh &mesh = meshes.back();
		return mesh;
	}

	Camera & Scene::addCamera(Node * parent)
	{
		cameras.emplace_back();
		Camera &camera = cameras.back();
		if (parent != nullptr)
			camera.setParent(parent);
		return camera;
	}

	Light & Scene::addLight(Node * parent)
	{
		lights.emplace_back();
		Light &light = lights.back();
		if (parent != nullptr)
			light.setParent(parent);
		return light;
	}

	Material & Scene::addMaterial()
	{
		materials.emplace_back();
		Material &material = materials.back();
		return material;
	}

	GL::Texture32 & Scene::addTexture(const std::vector<unsigned char> &data, unsigned int width, unsigned int height, unsigned int components)
	{
		textures.emplace_back(data, width, height, components);
		return textures.back();
	}

	Camera & Scene::getCurrentCamera()
	{
		return cameras[currentCamera];
	}

	bool Scene::GLTF::load(std::string path, Scene &scene)
	{
		tinygltf::Model tinyModel;
		tinygltf::TinyGLTF tinygltfctx;
		std::string err;
		bool ret = tinygltfctx.LoadASCIIFromFile(&tinyModel, &err, path);
		if (!err.empty())
			throw std::runtime_error("File failed loading : " + err);
		if (!ret)
			throw std::runtime_error("Failed to parse glTF");

		// --- TEXTURES
		scene.textures.reserve(tinyModel.textures.size());
		for (size_t iTex = 0; iTex < tinyModel.textures.size(); iTex++)
		{
			tinygltf::Texture &tinyTex = tinyModel.textures[iTex];
			tinygltf::Image &tinyImage = tinyModel.images[tinyTex.source];
			GL::Texture32 &newTex = scene.addTexture(tinyImage.image, tinyImage.width, tinyImage.height, tinyImage.component);
		}
		// --- MATERIALS
		scene.materials.reserve(tinyModel.materials.size());
		GL::Program *glProgram = new GL::Program();
		bool res = glProgram->attachShader(GL::Shader::VERTEX_SHADER, "../shaders/simpleShader.vert");
		ASSERT(res == true, "vertexShaderAttach failed");
		res = glProgram->attachShader(GL::Shader::FRAGMENT_SHADER, "../shaders/simpleShader.frag");
		ASSERT(res == true, "fragmentShaderAttach failed");
		res = glProgram->linkProgram();
		ASSERT(res == true, "LinkProgram failed");
		for (size_t iMaterial = 0; iMaterial < tinyModel.materials.size(); iMaterial++)
		{
			tinygltf::Material &tinyMat = tinyModel.materials[iMaterial];
			Material &newMat = scene.addMaterial();
			auto itColor = tinyMat.values.find("baseColorFactor");
			if (itColor == tinyMat.values.end())
				newMat.setColor(glm::vec4(1.f));
			else
			{
				tinygltf::ColorValue color = itColor->second.ColorFactor();
				newMat.setColor(glm::vec4(
					static_cast<float>(color[0]),
					static_cast<float>(color[1]),
					static_cast<float>(color[2]),
					static_cast<float>(color[3])
				));
			}
			auto itTexture = tinyMat.values.find("baseColorTexture");
			if (itTexture != tinyMat.values.end())
			{
				auto itIndex = itTexture->second.json_double_value.find("index");
				ASSERT(itIndex != itTexture->second.json_double_value.end(), "Index not defined");
				const unsigned int index = static_cast<unsigned int>(itIndex->second);
				newMat.setTexture(TextureType::COLOR_TEXTURE, &scene.textures[index]);
			}
			newMat.setProgram(glProgram);
		}
		// TODO implement primitives
		for (size_t iMesh = 0; iMesh < tinyModel.meshes.size(); iMesh++)
		{
			tinygltf::Mesh &tinyMesh = tinyModel.meshes[iMesh];
			Mesh &newMesh = scene.addMesh();
			for (size_t iPrimitive = 0; iPrimitive < tinyMesh.primitives.size(); iPrimitive++)
			{
				tinygltf::Primitive &tinyPrimitive = tinyMesh.primitives[iPrimitive];
				Primitive &newPrim = newMesh.addPrimitive();
				newPrim.material = &scene.materials[tinyPrimitive.material];
				// INDICES
				{
					tinygltf::Accessor &accessor = tinyModel.accessors[tinyPrimitive.indices];
					tinygltf::BufferView &bufferView = tinyModel.bufferViews[accessor.bufferView];
					tinygltf::Buffer &buffer = tinyModel.buffers[bufferView.buffer];
					int byteStride = accessor.ByteStride(bufferView);
					switch (accessor.type)
					{
					case TINYGLTF_TYPE_SCALAR:
						switch (accessor.componentType)
						{
						case TINYGLTF_COMPONENT_TYPE_BYTE:
						{
							int8_t data[3];
							ASSERT(accessor.count % 3 == 0, "Not moduloc correct");
							for (size_t iIndice = 0; iIndice < accessor.count; iIndice += 3)
							{
								memcpy(&data, &buffer.data[bufferView.byteOffset + accessor.byteOffset + iIndice * byteStride], sizeof(data));
								newPrim.triangles.push_back(Triangle(data[0], data[1], data[2]));
							}
						}
						break;
						case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
						{
							uint8_t data[3];
							ASSERT(accessor.count % 3 == 0, "Not moduloc correct");
							for (size_t iIndice = 0; iIndice < accessor.count; iIndice += 3)
							{
								memcpy(&data, &buffer.data[bufferView.byteOffset + accessor.byteOffset + iIndice * byteStride], sizeof(data));
								newPrim.triangles.push_back(Triangle(data[0], data[1], data[2]));
							}
						}
						break;
						case TINYGLTF_COMPONENT_TYPE_SHORT:
						{
							short data[3];
							ASSERT(accessor.count % 3 == 0, "Not moduloc correct");
							for (size_t iIndice = 0; iIndice < accessor.count; iIndice += 3)
							{
								memcpy(&data, &buffer.data[bufferView.byteOffset + accessor.byteOffset + iIndice * byteStride], sizeof(data));
								newPrim.triangles.push_back(Triangle(data[0], data[1], data[2]));
							}
						}
						break;
						case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
						{
							unsigned short data[3];
							ASSERT(accessor.count % 3 == 0, "Not moduloc correct");
							for (size_t iIndice = 0; iIndice < accessor.count; iIndice += 3)
							{
								memcpy(&data, &buffer.data[bufferView.byteOffset + accessor.byteOffset + iIndice * byteStride], sizeof(data));
								newPrim.triangles.push_back(Triangle(data[0], data[1], data[2]));
							}
						}
						break;
						case TINYGLTF_COMPONENT_TYPE_INT:
						{
							int data[3];
							ASSERT(accessor.count % 3 == 0, "Not moduloc correct");
							for (size_t iIndice = 0; iIndice < accessor.count; iIndice += 3)
							{
								memcpy(&data, &buffer.data[bufferView.byteOffset + accessor.byteOffset + iIndice * byteStride], sizeof(data));
								newPrim.triangles.push_back(Triangle(data[0], data[1], data[2]));
							}
						}
						break;
						case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
						{
							unsigned int data[3];
							ASSERT(accessor.count % 3 == 0, "Not moduloc correct");
							for (size_t iIndice = 0; iIndice < accessor.count; iIndice += 3)
							{
								memcpy(&data, &buffer.data[bufferView.byteOffset + accessor.byteOffset + iIndice * byteStride], sizeof(data));
								newPrim.triangles.push_back(Triangle(data[0], data[1], data[2]));
							}
						}
						}
						break;
					default:
						throw std::runtime_error("TINYGLTF Error : Accessor type not supported");
					}
				}

				// ATTRIBUTES
				{
					// POSITION
					{
						std::map<std::string, int>::iterator it = tinyPrimitive.attributes.find("POSITION");
						if (it != tinyPrimitive.attributes.end())
						{
							tinygltf::Accessor &accessor = tinyModel.accessors[it->second];
							tinygltf::BufferView &bufferView = tinyModel.bufferViews[accessor.bufferView];
							tinygltf::Buffer &buffer = tinyModel.buffers[bufferView.buffer];
							size_t size = accessor.count * tinygltf::GetTypeSizeInBytes(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
							ASSERT(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT, "Position not float");
							ASSERT(accessor.type == TINYGLTF_TYPE_VEC3, "Position not vec3");
							newPrim.vertices.reserve(accessor.count);
							for (size_t iVert = 0; iVert < accessor.count; iVert++)
							{
								newPrim.vertices.emplace_back();
								Vertex &vert = newPrim.vertices.back();
								memcpy(
									&vert.position,
									&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
									sizeof(glm::vec3)
								);
							}
						}
						else
						{
							throw std::runtime_error("No position attribute");
						}
					}
					// NORMAL
					{
						std::map<std::string, int>::iterator it = tinyPrimitive.attributes.find("NORMAL");
						if (it != tinyPrimitive.attributes.end())
						{
							tinygltf::Accessor &accessor = tinyModel.accessors[it->second];
							tinygltf::BufferView &bufferView = tinyModel.bufferViews[accessor.bufferView];
							tinygltf::Buffer &buffer = tinyModel.buffers[bufferView.buffer];
							size_t size = accessor.count * tinygltf::GetTypeSizeInBytes(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
							ASSERT(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT, "normal not float");
							ASSERT(accessor.type == TINYGLTF_TYPE_VEC3, "normal not vec3");
							for (size_t iVert = 0; iVert < accessor.count; iVert++)
							{
								Vertex &vert = newPrim.vertices[iVert];
								memcpy(
									&vert.normal,
									&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
									sizeof(glm::vec3)
								);
							}
						}
					}
					// TANGENT
					{
						std::map<std::string, int>::iterator it = tinyPrimitive.attributes.find("TANGENT");
						if (it != tinyPrimitive.attributes.end())
						{
							tinygltf::Accessor &accessor = tinyModel.accessors[it->second];
							tinygltf::BufferView &bufferView = tinyModel.bufferViews[accessor.bufferView];
							tinygltf::Buffer &buffer = tinyModel.buffers[bufferView.buffer];
							size_t size = accessor.count * tinygltf::GetTypeSizeInBytes(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
							ASSERT(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT, "TANGENT not float");
							ASSERT(accessor.type == TINYGLTF_TYPE_VEC4, "TANGENT not vec3");
							/*for (size_t iVert = 0; iVert < accessor.count; iVert++)
							{
							Vertex &vert = newPrim.vertices[iVert];
							memcpy(
							vert.normal.data,
							&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
							sizeof(point3)
							);
							}*/
						}
					}
					// TEXCOORD_0
					{
						std::map<std::string, int>::iterator it = tinyPrimitive.attributes.find("TEXCOORD_0");
						if (it != tinyPrimitive.attributes.end())
						{
							tinygltf::Accessor &accessor = tinyModel.accessors[it->second];
							tinygltf::BufferView &bufferView = tinyModel.bufferViews[accessor.bufferView];
							tinygltf::Buffer &buffer = tinyModel.buffers[bufferView.buffer];
							size_t size = accessor.count * tinygltf::GetTypeSizeInBytes(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
							ASSERT(accessor.type == TINYGLTF_TYPE_VEC2, "TEXCOORD_0 not vec2");
							switch (accessor.componentType)
							{
							case TINYGLTF_COMPONENT_TYPE_FLOAT:
								for (size_t iVert = 0; iVert < accessor.count; iVert++)
								{
									Vertex &vert = newPrim.vertices[iVert];
									memcpy(
										&vert.texCoord[0],
										&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
										sizeof(glm::vec2)
									);
								}
								break;
							case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
								ASSERT(accessor.normalized == true, "Must be normalized");
								//break;
							case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
								ASSERT(accessor.normalized == true, "Must be normalized");
								//break;
							default:
								throw std::runtime_error("Component type for texCoord not supported");
							}
						}
					}
					// TEXCOORD_1
					{
						std::map<std::string, int>::iterator it = tinyPrimitive.attributes.find("TEXCOORD_1");
						if (it != tinyPrimitive.attributes.end())
						{
							tinygltf::Accessor &accessor = tinyModel.accessors[it->second];
							tinygltf::BufferView &bufferView = tinyModel.bufferViews[accessor.bufferView];
							tinygltf::Buffer &buffer = tinyModel.buffers[bufferView.buffer];
							size_t size = accessor.count * tinygltf::GetTypeSizeInBytes(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
							ASSERT(accessor.type == TINYGLTF_TYPE_VEC2, "TEXCOORD_1 not vec2");
							switch (accessor.componentType)
							{
							case TINYGLTF_COMPONENT_TYPE_FLOAT:
								//break;
							case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
								ASSERT(accessor.normalized == true, "Must be normalized");
								//break;
							case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
								ASSERT(accessor.normalized == true, "Must be normalized");
								//break;
							default:
								throw std::runtime_error("Component type for texCoord not supported");
							}
						}
					}
					// COLOR_0
					{
						std::map<std::string, int>::iterator it = tinyPrimitive.attributes.find("COLOR_0");
						if (it != tinyPrimitive.attributes.end())
						{
							tinygltf::Accessor &accessor = tinyModel.accessors[it->second];
							tinygltf::BufferView &bufferView = tinyModel.bufferViews[accessor.bufferView];
							tinygltf::Buffer &buffer = tinyModel.buffers[bufferView.buffer];
							size_t size = accessor.count * tinygltf::GetTypeSizeInBytes(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
							ASSERT(accessor.type == TINYGLTF_TYPE_VEC4, "COLOR_0 not vec4");
							switch (accessor.type)
							{
							case TINYGLTF_TYPE_VEC3:
							{
								switch (accessor.componentType)
								{
								case TINYGLTF_COMPONENT_TYPE_FLOAT:
									for (size_t iVert = 0; iVert < accessor.count; iVert++)
									{
										Vertex &vert = newPrim.vertices[iVert];
										memcpy(
											&vert.color,
											&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
											sizeof(glm::vec3)
										);
										vert.color.w = 1.f;
									}
									break;
								case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
									ASSERT(accessor.normalized == true, "Must be normalized");
									//break;
								case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
									ASSERT(accessor.normalized == true, "Must be normalized");
									//break;
								default:
									throw std::runtime_error("Component type for texCoord not supported");
								}
							}
							break;
							case TINYGLTF_TYPE_VEC4:
							{
								switch (accessor.componentType)
								{
								case TINYGLTF_COMPONENT_TYPE_FLOAT:
									for (size_t iVert = 0; iVert < accessor.count; iVert++)
									{
										Vertex &vert = newPrim.vertices[iVert];
										memcpy(
											&vert.color,
											&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
											sizeof(glm::vec4)
										);
									}
									break;
								case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
									ASSERT(accessor.normalized == true, "Must be normalized");
									//break;
								case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
									ASSERT(accessor.normalized == true, "Must be normalized");
									//break;
								default:
									throw std::runtime_error("Component type for texCoord not supported");
								}
							}
							break;
							default:
								throw std::runtime_error("Accessor type incorrect");
							}
						}
						else // NO COLORS. set defaults
						{
							for (size_t iVert = 0; iVert < newPrim.vertices.size(); iVert++)
							{
								Vertex &vert = newPrim.vertices[iVert];
								vert.color.x = 1.f;
								vert.color.y = 1.f;
								vert.color.z = 1.f;
								vert.color.w = 1.f;
							}
						}
					}
				}
				bool res = newPrim.createVAO();
				ASSERT(res == true, "VAO not created");
			}
		}
		for (size_t iNode = 0; iNode < tinyModel.nodes.size(); iNode++)
		{
			tinygltf::Node &tinyNode = tinyModel.nodes[iNode];
			Node &newNode = scene.addNode();
			glm::mat4 localTransform;
			if (tinyNode.matrix.size() == 16)
			{
				for (int col = 0; col < 4; col++)
					for (int row = 0; row < 4; row++)
						localTransform[col][row] = static_cast<float>(tinyNode.matrix[col * 4 + row]);
			}
			else
			{
				glm::mat4 translation(1.f);
				glm::quat rotation(1.f, 0.f, 0.f, 0.f);
				glm::mat4 scale(1.f);

				if (tinyNode.translation.size() == 3)
					for (int i = 0; i < 3; i++)
						translation[3][i] = static_cast<float>(tinyNode.translation[i]);

				if (tinyNode.scale.size() == 3)
					for (int i = 0; i < 3; i++)
						scale[i][i] = static_cast<float>(tinyNode.scale[i]);

				if (tinyNode.rotation.size() == 4)
					for (int i = 0; i < 4; i++)
						rotation[i] = static_cast<float>(tinyNode.rotation[i]);

				localTransform = translation * glm::mat4_cast(rotation) * scale;
			}
			newNode.setLocalTransform(localTransform);
			if (tinyNode.mesh != -1)
				newNode.setMesh(&scene.meshes[tinyNode.mesh]);
		}
		// Child nodes
		for (size_t iNode = 0; iNode < tinyModel.nodes.size(); iNode++)
		{
			tinygltf::Node &tinyNode = tinyModel.nodes[iNode];
			Node &node = scene.nodes[iNode];
			for (size_t iChild = 0; iChild < tinyNode.children.size(); iChild++)
			{
				const int childID = tinyNode.children[iChild];
				Node &childNode = scene.nodes[childID];
				childNode.setParent(&node);
				node.addChild(&childNode);
			}
		}

		const size_t nbScene = tinyModel.scenes.size();
		for (size_t iNode = 0; iNode < tinyModel.scenes[0].nodes.size(); iNode++)
		{
			scene.roots.push_back(&scene.nodes[tinyModel.scenes[0].nodes[iNode]]);
		}

		Camera &camera = scene.addCamera();
		camera.setLocalTransform(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 1.f)));
		scene.currentCamera = 0;

		return true;
	}

	bool Scene::CUSTOM::load(std::string path, Scene &scene)
	{
		// Program
		GL::Program *glProgram = new GL::Program();
		bool res = glProgram->attachShader(GL::Shader::VERTEX_SHADER, "../shaders/simpleShader.vert");
		ASSERT(res == true, "vertexShaderAttach failed");
		res = glProgram->attachShader(GL::Shader::FRAGMENT_SHADER, "../shaders/simpleShader.frag");
		ASSERT(res == true, "fragmentShaderAttach failed");
		res = glProgram->linkProgram();
		ASSERT(res == true, "LinkProgram failed");

		// Node
		Node &root = scene.addNode();
		scene.roots.push_back(&root);

		Mesh &mesh = scene.addMesh();
		root.setMesh(&mesh);
		Primitive &prim = mesh.addPrimitive();
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
					prim.vertices.push_back(vert);
				}
			}
		}
		prim.triangles.push_back(Triangle(0, 1, 2));
		prim.triangles.push_back(Triangle(2, 1, 3));
		prim.triangles.push_back(Triangle(2, 7, 3));
		prim.triangles.push_back(Triangle(6, 7, 2));
		prim.triangles.push_back(Triangle(3, 5, 1));
		prim.triangles.push_back(Triangle(7, 5, 3));
		prim.triangles.push_back(Triangle(0, 5, 1));
		prim.triangles.push_back(Triangle(5, 4, 0));
		prim.triangles.push_back(Triangle(6, 4, 2));
		prim.triangles.push_back(Triangle(2, 4, 0));
		prim.triangles.push_back(Triangle(6, 4, 7));
		prim.triangles.push_back(Triangle(7, 4, 5));

		res = prim.createVAO();
		ASSERT(res == true, "VAO failed creation");

		// Material
		Material &material = scene.addMaterial();
		material.setProgram(glProgram);
		
		// Texture
		Image image("../textures/ocean.jpg");
		GL::Texture32 &texture = scene.addTexture(
			std::vector<unsigned char>(image.data(), image.data() + image.size()), 
			image.width(), 
			image.height(), 
			image.componentSize()
		);
		// LEAK HERE
		material.setTexture(TextureType::COLOR_TEXTURE, &texture);
		prim.material = &material;

		// Camera
		Camera &camera = scene.addCamera();
		camera.setLocalTransform(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 1.f)));
		scene.currentCamera = 0;

		return true;
	}

}