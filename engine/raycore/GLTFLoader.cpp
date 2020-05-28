#include "GLTFLoader.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "tiny_gltf.h"

#include "Triangle.h"

namespace raycore {

bool GLTFLoader::load(Reader & reader, prim::Scene & scene)
{
	tinygltf::TinyGLTF ctx;
	tinygltf::Model tinyModel;
	std::string err;
	// TODO fix base dir
	bool isLoaded = ctx.LoadASCIIFromString(&tinyModel, &err, reinterpret_cast<const char*>(reader.data()), reader.size(), "./");
	if (!isLoaded)
	{
		throw std::runtime_error("File failed loading");
	}
	if (!err.empty())
	{
		throw std::runtime_error(err);
	}
	std::vector<prim::Hitable*> prims(tinyModel.meshes.size());
	std::vector<prim::Material*> materials(tinyModel.materials.size());
	std::vector<Texture<colorHDR>*> textures(tinyModel.textures.size());
	const size_t nbScene = tinyModel.scenes.size();
	// TODO manage multiple scenes
	// --- TEXTURES
	//scene.textures.reserve(tinyModel.textures.size());
	for (size_t iTex = 0; iTex < tinyModel.textures.size(); iTex++)
	{
		tinygltf::Texture &tinyTex = tinyModel.textures[iTex];
		tinygltf::Image &tinyImage = tinyModel.images[tinyTex.source];
		textures[iTex] = scene.addTexture(new TextureMap32(tinyImage.image, tinyImage.width, tinyImage.height, tinyImage.component));
	}
	// --- MATERIALS
	for (size_t iMaterial = 0; iMaterial < tinyModel.materials.size(); iMaterial++)
	{
		tinygltf::Material &tinyMat = tinyModel.materials[iMaterial];

		colorHDR color = colorHDR(1.f);
		auto itColor = tinyMat.values.find("baseColorFactor");
		if (itColor != tinyMat.values.end())
		{
			tinygltf::ColorValue c = itColor->second.ColorFactor();
			color = colorHDR(
				static_cast<float>(c[0]),
				static_cast<float>(c[1]),
				static_cast<float>(c[2]),
				static_cast<float>(c[3])
			);
		}
		int textureID = -1;
		auto itTexture = tinyMat.values.find("baseColorTexture");
		if (itTexture != tinyMat.values.end())
		{
			auto itIndex = itTexture->second.json_double_value.find("index");
			ASSERT(itIndex != itTexture->second.json_double_value.end(), "Index not defined");
			textureID = static_cast<int>(itIndex->second);
		}
		Texture<colorHDR> *colorText;
		if (textureID == -1)
		{
			colorText = new ConstantTexture<colorHDR>(color);
		}
		else
		{
			colorText = textures[textureID];
		}
		materials[iMaterial] = scene.addMaterial(new prim::Matte(colorText));
	}

	auto coordinates = [](const point3 &p) -> point3
	{
		return point3(p.x, -p.z, p.y);
	};
	auto coordinatesN = [](const norm3 &p) -> norm3
	{
		return norm3(p.x, -p.z, p.y);
	};

	// --- MESHES
	tinygltf::Buffer &buffer = tinyModel.buffers[0];
	std::vector<prim::Hitable*> hitables;

	for (size_t iMesh = 0; iMesh < tinyModel.meshes.size(); iMesh++)
	{
		tinygltf::Mesh &tinyMesh = tinyModel.meshes[iMesh];
		for (size_t iPrimitive = 0; iPrimitive < tinyMesh.primitives.size(); iPrimitive++)
		{
			tinygltf::Primitive &tinyPrimitive = tinyMesh.primitives[iPrimitive];

			std::vector<prim::Vertex> vertices;
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
						vertices.reserve(accessor.count);
						for (size_t iVert = 0; iVert < accessor.count; iVert++)
						{
							prim::Vertex vert;
							memcpy(
								vert.position.data,
								&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
								sizeof(point3)
							);
							vert.position = coordinates(vert.position);
							vertices.push_back(vert);
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
							prim::Vertex &vert = vertices[iVert];
							memcpy(
								vert.normal.data,
								&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
								sizeof(point3)
							);
							vert.normal = coordinatesN(vert.normal);
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
						Vertex &vert = newMesh.vertices[iVert];
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
								prim::Vertex &vert = vertices[iVert];
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
									prim::Vertex &vert = vertices[iVert];
									memcpy(
										vert.color.data,
										&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
										sizeof(float) * 3
									);
									vert.color.a = 1.f;
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
									prim::Vertex &vert = vertices[iVert];
									memcpy(
										vert.color.data,
										&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
										sizeof(colorHDR)
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
						for (size_t iVert = 0; iVert < vertices.size(); iVert++)
						{
							prim::Vertex &vert = vertices[iVert];
							vert.color.r = 1.f;
							vert.color.g = 1.f;
							vert.color.b = 1.f;
							vert.color.a = 1.f;
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
							prims.push_back(new prim::Triangle(
								vertices[data[0]],
								vertices[data[1]],
								vertices[data[2]]
							));
							ASSERT(static_cast<size_t>(data[0]) < vertices.size(), "what");
							ASSERT(static_cast<size_t>(data[1]) < vertices.size(), "what");
							ASSERT(static_cast<size_t>(data[2]) < vertices.size(), "what");
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
							prims.push_back(new prim::Triangle(
								vertices[data[0]],
								vertices[data[1]],
								vertices[data[2]]
							));
							ASSERT(static_cast<size_t>(data[0]) < vertices.size(), "what");
							ASSERT(static_cast<size_t>(data[1]) < vertices.size(), "what");
							ASSERT(static_cast<size_t>(data[2]) < vertices.size(), "what");
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
							prims.push_back(new prim::Triangle(
								vertices[data[0]],
								vertices[data[1]],
								vertices[data[2]]
							));
							ASSERT(static_cast<size_t>(data[0]) < vertices.size(), "what");
							ASSERT(static_cast<size_t>(data[1]) < vertices.size(), "what");
							ASSERT(static_cast<size_t>(data[2]) < vertices.size(), "what");
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
							prims.push_back(new prim::Triangle(
								vertices[data[0]],
								vertices[data[1]],
								vertices[data[2]]
							));
							ASSERT(static_cast<size_t>(data[0]) < vertices.size(), "what");
							ASSERT(static_cast<size_t>(data[1]) < vertices.size(), "what");
							ASSERT(static_cast<size_t>(data[2]) < vertices.size(), "what");
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
							prims.push_back(new prim::Triangle(
								vertices[data[0]],
								vertices[data[1]],
								vertices[data[2]]
							));
							ASSERT(static_cast<size_t>(data[0]) < vertices.size(), "what");
							ASSERT(static_cast<size_t>(data[1]) < vertices.size(), "what");
							ASSERT(static_cast<size_t>(data[2]) < vertices.size(), "what");
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
							prims.push_back(new prim::Triangle(
								vertices[data[0]],
								vertices[data[1]],
								vertices[data[2]]
							));
							ASSERT(static_cast<size_t>(data[0]) < vertices.size(), "what");
							ASSERT(static_cast<size_t>(data[1]) < vertices.size(), "what");
							ASSERT(static_cast<size_t>(data[2]) < vertices.size(), "what");
						}
					}
					}
					break;
				default:
					throw std::runtime_error("TINYGLTF Error : Accessor type not supported");
				}
			}
		}
	}

	// --- Nodes
	std::vector<prim::Node*> nodes;
	for (size_t iNode = 0; iNode < tinyModel.nodes.size(); iNode++)
	{
		tinygltf::Node &tinyNode = tinyModel.nodes[iNode];
		prim::Node *node;
		if (tinyNode.mesh != -1) // GEOMETRY
		{
			node = new prim::Geometry(prims);
			node->setAcceleration(prim::Acceleration::BVH);
			nodes.push_back(node);
		}
		else if (tinyNode.children.size() > 0) // GROUP
		{
			node = new prim::Group();
			node->setAcceleration(prim::Acceleration::NONE);
			nodes.push_back(node);
		}
		else // ERROR
		{
			Log::warn("Node should have a mesh or a child. Skipping");
			nodes.push_back(nullptr);
			continue;
		}
		if (tinyNode.matrix.size() == 16)
		{
			int globalIndex = 0;
			mat4 transform;
			for (int col = 0; col < 4; col++)
				for (int row = 0; row < 4; row++)
					transform[col][row] = static_cast<float>(tinyNode.matrix[globalIndex++]);
			node->setTransform(transform);
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

			node->setTransform(TRS(translation, rotation, scale));
		}
	}
	// Child nodes
	for (size_t iNode = 0; iNode < tinyModel.nodes.size(); iNode++)
	{
		tinygltf::Node &tinyNode = tinyModel.nodes[iNode];
		if (tinyNode.children.size() == 0)
			continue;

		if (tinyNode.mesh != -1)
		{
			Log::error("Should not have mesh");
		}
		else
		{
			prim::Group* group = dynamic_cast<prim::Group*>(nodes[iNode]);
			ASSERT(group != nullptr, "SHould not be null");
			for (size_t iChild = 0; iChild < tinyNode.children.size(); iChild++)
			{
				const int childID = tinyNode.children[iChild];
				if (nodes[childID] != nullptr)
					group->addChild(nodes[childID]);
			}
		}
	}
	prim::Group* root = dynamic_cast<prim::Group*>(nodes[tinyModel.scenes[0].nodes[0]]);
	if (root == nullptr)
	{
		root = new prim::Group();
		for (size_t iNode = 0; iNode < tinyModel.scenes[tinyModel.defaultScene].nodes.size(); iNode++)
			root->addChild(nodes[tinyModel.scenes[tinyModel.defaultScene].nodes[iNode]]);
	}
	scene.setRoot(root);
	scene.setLightDistribution(new prim::LightDistribution);
	return true;
}

bool GLTFLoader::write(Writer & writer, const prim::Scene & scene)
{
	throw std::runtime_error("Not implemented");
}

}