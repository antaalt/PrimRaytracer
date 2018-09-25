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
	Primitive & Scene::addPrimitive()
	{
		primitives.emplace_back();
		return primitives.back();
	}
	Mesh & Scene::addMesh()
	{
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
	Parallelogram & Scene::addParallelogram()
	{
		Parallelogram *p = new Parallelogram();
		shapes.push_back(p);
		return *p;
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
	Texture & Scene::addTexture(const std::vector<unsigned char> &data, unsigned int width, unsigned int height, unsigned int components)
	{
		textures.emplace_back(data, width, height, components);
		return textures.back();
	}
	Light & Scene::addLight()
	{
		lights.emplace_back();
		return lights.back();
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
		// --- TEXTURES
		scene.textures.reserve(tinyModel.textures.size());
		for (size_t iTex = 0; iTex < tinyModel.textures.size(); iTex++)
		{
			tinygltf::Texture &tinyTex = tinyModel.textures[iTex];
			tinygltf::Image &tinyImage = tinyModel.images[tinyTex.source];
			Texture &newTex = scene.addTexture(tinyImage.image, tinyImage.width, tinyImage.height, tinyImage.component);
		}
		// --- MATERIALS
		scene.materials.reserve(tinyModel.materials.size());
		for (size_t iMaterial = 0; iMaterial < tinyModel.materials.size(); iMaterial++)
		{
			tinygltf::Material &tinyMat = tinyModel.materials[iMaterial];
			Material &newMat = scene.addMaterial();
			newMat.index = iMaterial;
			auto itColor = tinyMat.values.find("baseColorFactor");
			if (itColor == tinyMat.values.end())
				newMat.color = color4(1.f);
			else
			{
				tinygltf::ColorValue color = itColor->second.ColorFactor();
				newMat.color = color4(
					static_cast<float>(color[0]),
					static_cast<float>(color[1]),
					static_cast<float>(color[2]),
					static_cast<float>(color[3])
				);
			}
			auto itTexture = tinyMat.values.find("baseColorTexture");
			if (itTexture == tinyMat.values.end())
				newMat.texture = nullptr;
			else
			{
				auto itIndex = itTexture->second.json_double_value.find("index");
				ASSERT(itIndex != itTexture->second.json_double_value.end(), "Index not defined");
				const unsigned int index = static_cast<unsigned int>(itIndex->second);
				newMat.texture = &scene.textures[index];
			}
		}
		// --- MESHES
		tinygltf::Buffer &buffer = tinyModel.buffers[0];
		size_t sizePrim = 0;
		for (size_t iMesh = 0; iMesh < tinyModel.meshes.size(); iMesh++)
			sizePrim += tinyModel.meshes[iMesh].primitives.size();
		scene.primitives.reserve(sizePrim);
		for (size_t iMesh = 0; iMesh < tinyModel.meshes.size(); iMesh++)
		{
			tinygltf::Mesh &tinyMesh = tinyModel.meshes[iMesh];
			Mesh &newMesh = scene.addMesh();
			for (size_t iPrimitive = 0; iPrimitive < tinyMesh.primitives.size(); iPrimitive++)
			{
				tinygltf::Primitive &tinyPrimitive = tinyMesh.primitives[iPrimitive];
				Primitive &newPrim = scene.addPrimitive();
				newPrim.material = &scene.materials[tinyPrimitive.material];
				
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
									sizeof(point3)
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
									vert.normal.data,
									&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
									sizeof(point3)
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
							//throw std::runtime_error("Tangents not implemented");
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
										vert.texcoord.data,
										&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
										sizeof(uv2)
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
											sizeof(color4)
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
								ASSERT(static_cast<size_t>(data[0]) < newPrim.vertices.size(), "what");
								ASSERT(static_cast<size_t>(data[1]) < newPrim.vertices.size(), "what");
								ASSERT(static_cast<size_t>(data[2]) < newPrim.vertices.size(), "what");
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
								ASSERT(static_cast<size_t>(data[0]) < newPrim.vertices.size(), "what");
								ASSERT(static_cast<size_t>(data[1]) < newPrim.vertices.size(), "what");
								ASSERT(static_cast<size_t>(data[2]) < newPrim.vertices.size(), "what");
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
								ASSERT(static_cast<size_t>(data[0]) < newPrim.vertices.size(), "what");
								ASSERT(static_cast<size_t>(data[1]) < newPrim.vertices.size(), "what");
								ASSERT(static_cast<size_t>(data[2]) < newPrim.vertices.size(), "what");
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
								ASSERT(static_cast<size_t>(data[0]) < newPrim.vertices.size(), "what");
								ASSERT(static_cast<size_t>(data[1]) < newPrim.vertices.size(), "what");
								ASSERT(static_cast<size_t>(data[2]) < newPrim.vertices.size(), "what");
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
								ASSERT(static_cast<size_t>(data[0]) < newPrim.vertices.size(), "what");
								ASSERT(static_cast<size_t>(data[1]) < newPrim.vertices.size(), "what");
								ASSERT(static_cast<size_t>(data[2]) < newPrim.vertices.size(), "what");
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
								ASSERT(static_cast<size_t>(data[0]) < newPrim.vertices.size(), "what");
								ASSERT(static_cast<size_t>(data[1]) < newPrim.vertices.size(), "what");
								ASSERT(static_cast<size_t>(data[2]) < newPrim.vertices.size(), "what");
							}
						}
						}
						break;
					default:
						throw std::runtime_error("TINYGLTF Error : Accessor type not supported");
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
				vec3 translation(0.f);
				quat rotation = quat::identity();
				vec3 scale(1.f);

				if (tinyNode.translation.size() == 3)
					for (int i = 0; i < 3; i++)
						translation[i] = static_cast<float>(tinyNode.translation[i]);

				if (tinyNode.scale.size() == 3)
					for (int i = 0; i < 3; i++)
						scale[i] = static_cast<float>(tinyNode.scale[i]);

				if (tinyNode.rotation.size() == 4)
					for (int i = 0; i < 4; i++)
						rotation[i] = static_cast<float>(tinyNode.rotation[i]);

				node.transform = mat4::TRS(translation, rotation, scale);
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
	mat4 Node::getModel() const
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
	ShapeType Parallelogram::type() const
	{
		return ShapeType::PARALLELOGRAM;
	}
}

