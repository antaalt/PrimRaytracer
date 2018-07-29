#include "Scene.h"

#include "Config.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "tiny_gltf.h"

namespace app {
	Triangle::Triangle()
	{
	}
	Triangle::Triangle(unsigned int v1, unsigned int v2, unsigned int v3): A(v1), B(v2), C(v3)
	{
	}
	Texture::Texture()
	{
	}
	Texture::Texture(const Image & image)
	{
		// TODO convert image to textures
	}

	ColorHDR Texture::texture2D(float u, float v)
	{
		unsigned int uPixel = static_cast<unsigned int>(m_width / u);
		unsigned int vPixel = static_cast<unsigned int>(m_height / u); // TODO linear and co
		unsigned int index = vPixel * m_width + uPixel;
		return ColorHDR(m_data[index], m_data[index + 1], m_data[index + 2], m_data[index + 3]);
	}

	unsigned int Texture::stride()
	{
		return sizeof(float) * m_component;
	}

	Primitive & Scene::addPrimitive()
	{
		primitives.emplace_back();
		return primitives.back();
	}
	Mesh & Scene::addMesh()
	{
		/*meshes.emplace_back();
		return meshes.back();*/
		Mesh *m = new Mesh();
		shapes.push_back(m);
		return *m;
	}
	Sphere & Scene::addSphere()
	{
		Sphere *s = new Sphere();
		shapes.push_back(s);
		return *s;
	}
	Node & Scene::addNode()
	{
		nodes.emplace_back();
		return nodes.back();
	}
	Material & Scene::addMaterial()
	{
		materials.emplace_back();
		return materials.back();
	}
	Texture & Scene::addTexture()
	{
		textures.emplace_back();
		return textures.back();
	}
	Scene Scene::GLTF::load(std::string path)
	{
		tinygltf::TinyGLTF ctx;
		tinygltf::Model tinyModel;
		std::string err;
		bool isLoaded = ctx.LoadASCIIFromFile(&tinyModel, &err, path);
		if (!isLoaded)
		{
			throw std::runtime_error("File failed loading");
		}
		if (!err.empty())
		{
			throw std::runtime_error(err);
		}
		Scene scene;
		const size_t nbScene = tinyModel.scenes.size();
		// TODO manage multiple scenes
		scene.materials.reserve(tinyModel.materials.size());
		// --- MATERIALS
		for (size_t iMaterial = 0; iMaterial < tinyModel.materials.size(); iMaterial++)
		{
			tinygltf::Material &tinyMat = tinyModel.materials[iMaterial];
			Material &newMat = scene.addMaterial();
			newMat.index = iMaterial;
			auto itColor = tinyMat.values.find("baseColorFactor");
			if (itColor == tinyMat.values.end())
				newMat.color = ColorHDR(1.f);
			else
			{
				tinygltf::ColorValue color = itColor->second.ColorFactor();
				newMat.color = ColorHDR(
					static_cast<float>(color[0]),
					static_cast<float>(color[1]),
					static_cast<float>(color[2]),
					static_cast<float>(color[3])
				);
			}
		}
		// --- MESHES
		tinygltf::Buffer &buffer = tinyModel.buffers[0];
		for (size_t iMesh = 0; iMesh < tinyModel.meshes.size(); iMesh++)
		{
			tinygltf::Mesh &tinyMesh = tinyModel.meshes[iMesh];
			Mesh &newMesh = scene.addMesh();
			for (size_t iPrimitive = 0; iPrimitive < tinyMesh.primitives.size(); iPrimitive++)
			{
				tinygltf::Primitive &tinyPrimitive = tinyMesh.primitives[iPrimitive];
				Primitive &newPrim = scene.addPrimitive();
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
									vert.position.data, 
									&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)], 
									sizeof(Point3)
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
							ASSERT(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT, "Normal not float");
							ASSERT(accessor.type == TINYGLTF_TYPE_VEC3, "Normal not vec3");
							for (size_t iVert = 0; iVert < accessor.count; iVert++)
							{
								Vertex &vert = newPrim.vertices[iVert];
								memcpy(
									vert.normal.data,
									&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
									sizeof(Point3)
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
									sizeof(Point3)
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
								break;
							case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
								ASSERT(accessor.normalized == true, "Must be normalized");
								break;
							case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
								ASSERT(accessor.normalized == true, "Must be normalized");
								break;
							default:
								throw std::runtime_error("Component type for texCoord not supported");
							}
							/*for (size_t iVert = 0; iVert < accessor.count; iVert++)
							{
							Vertex &vert = newPrim.vertices[iVert];
							memcpy(
							vert.normal.data,
							&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
							sizeof(Point3)
							);
							}*/
						}
					}
					// TEXCOORD_1
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
								break;
							case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
								ASSERT(accessor.normalized == true, "Must be normalized");
								break;
							case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
								ASSERT(accessor.normalized == true, "Must be normalized");
								break;
							default:
								throw std::runtime_error("Component type for texCoord not supported");
							}
							/*for (size_t iVert = 0; iVert < accessor.count; iVert++)
							{
							Vertex &vert = newPrim.vertices[iVert];
							memcpy(
							vert.normal.data,
							&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
							sizeof(Point3)
							);
							}*/
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
											vert.color.data,
											&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
											sizeof(float) * 3
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
											vert.color.data,
											&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
											sizeof(ColorHDR)
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
								
							/*for (size_t iVert = 0; iVert < accessor.count; iVert++)
							{
							Vertex &vert = newPrim.vertices[iVert];
							memcpy(
							vert.normal.data,
							&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
							sizeof(Point3)
							);
							}*/
						}
					}
				}
				// TODO mode
				newMesh.primitives.push_back(&newPrim);
			}
		}

		// --- Nodes
		scene.nodes.reserve(tinyModel.nodes.size());
		for (size_t iNode = 0; iNode < tinyModel.nodes.size(); iNode++)
		{
			tinygltf::Node &tinyNode = tinyModel.nodes[iNode];
			Node &node = scene.addNode();
			if (tinyNode.mesh != -1)
				node.shape = scene.shapes[tinyNode.mesh];
			else
				node.shape = nullptr;

			if (tinyNode.matrix.size() == 16)
			{
				int globalIndex = 0;
				for (int col = 0; col < 4; col++)
					for (int row = 0; row < 4; row++)
						node.transform[col][row] = static_cast<float>(tinyNode.matrix[globalIndex++]);
			}
			else
			{
				Vector3 translation(0.f);
				Quaternion rotation = Quaternion::identity();
				Vector3 scale(1.f);

				if (tinyNode.translation.size() == 3)
					for (int i = 0; i < 3; i++)
						translation[i] = static_cast<float>(tinyNode.translation[i]);

				if (tinyNode.scale.size() == 3)
					for (int i = 0; i < 3; i++)
						scale[i] = static_cast<float>(tinyNode.scale[i]);

				if (tinyNode.rotation.size() == 4)
					for (int i = 0; i < 4; i++)
						rotation[i] = static_cast<float>(tinyNode.rotation[i]);

				node.transform = Matrix4::TRS(translation, rotation, scale);
			}
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
				childNode.parent = &node;
				node.childrens.push_back(&childNode);
			}
		}
		return scene;
	}
	bool Scene::GLTF::write(const Scene & scene)
	{
		return false;
	}
	Matrix4 Node::getModel() const
	{
		if (parent == nullptr)
			return transform;
		return parent->getModel() * transform;
	}
	ShapeType Sphere::type() const
	{
		return ShapeType::SPHERE;
	}
	ShapeType Mesh::type() const
	{
		return ShapeType::MESH;
	}
	ShapeType Plane::type() const
	{
		return ShapeType::PLANE;
	}
}

