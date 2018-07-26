#include "Scene.h"

#define TINY_GLTF_IMPLEMENTATION
#include "tiny_gltf.h"

namespace Application {
	namespace RayTracer {

		Scene Scene::GLTF::load(std::string path)
		{
			tinygltf::TinyGLTF ctx;
			tinygltf::Model tinyModel;
			std::string err;
			bool isLoaded = ctx.LoadASCIIFromFile(&tinyModel, &err, path);
			if (!isLoaded)
			{
				Log::error("File failed loading");
				return Scene();
			}
			if (!err.empty())
			{
				Log::error(err);
				return Scene();
			}
			Scene scene;
			const size_t nbScene = tinyModel.scenes.size();
			// TODO manage multiple scenes
			scene.materials.reserve(tinyModel.materials.size());
			// --- MATERIALS
			for (size_t iMaterial = 0; iMaterial < tinyModel.materials.size(); iMaterial++)
			{
				tinygltf::Material &tinyMat = tinyModel.materials[iMaterial];
				//Material::Ptr newMat;
				// TODO materials
			}
			// --- MESHES
			tinygltf::Buffer &buffer = tinyModel.buffers[0];
			for (size_t iMesh = 0; iMesh < tinyModel.meshes.size(); iMesh++)
			{
				tinygltf::Mesh &tinyMesh = tinyModel.meshes[iMesh];
				scene.primitives.emplace_back();
				Primitive &newPrim = scene.primitives.back();
				for (size_t iPrimitive = 0; iPrimitive < tinyMesh.primitives.size(); iPrimitive++)
				{
					tinygltf::Primitive &tinyPrimitive = tinyMesh.primitives[iPrimitive];
					// INDICES
					{
						tinygltf::Accessor &accessor = tinyModel.accessors[tinyPrimitive.indices];
						tinygltf::BufferView &bufferView = tinyModel.bufferViews[accessor.bufferView];
						tinygltf::Buffer &buffer = tinyModel.buffers[bufferView.buffer];
						size_t size = accessor.count * tinygltf::GetTypeSizeInBytes(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
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
									memcpy(&data, &buffer.data[bufferView.byteOffset + accessor.byteOffset + iIndice], 1);
									newPrim.triangles.emplace_back(data[0], data[1], data[2]);
								}
							}
							break;
							case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
							{
								uint8_t data[3];
								ASSERT(accessor.count % 3 == 0, "Not moduloc correct");
								for (size_t iIndice = 0; iIndice < accessor.count; iIndice += 3)
								{
									memcpy(&data, &buffer.data[bufferView.byteOffset + accessor.byteOffset + iIndice], 1);
									newPrim.triangles.emplace_back(data[0], data[1], data[2]);
								}
							}
							break;
							case TINYGLTF_COMPONENT_TYPE_SHORT:
							{
								short data[3];
								ASSERT(accessor.count % 3 == 0, "Not moduloc correct");
								for (size_t iIndice = 0; iIndice < accessor.count; iIndice += 3)
								{
									memcpy(&data, &buffer.data[bufferView.byteOffset + accessor.byteOffset + iIndice], 2);
									newPrim.triangles.emplace_back(data[0], data[1], data[2]);
								}
							}
							break;
							case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
							{
								unsigned short data[3];
								ASSERT(accessor.count % 3 == 0, "Not moduloc correct");
								for (size_t iIndice = 0; iIndice < accessor.count; iIndice += 3)
								{
									memcpy(&data, &buffer.data[bufferView.byteOffset + accessor.byteOffset + iIndice], 2);
									newPrim.triangles.emplace_back(data[0], data[1], data[2]);
								}
							}
							break;
							case TINYGLTF_COMPONENT_TYPE_INT:
							{
								int data[3];
								ASSERT(accessor.count % 3 == 0, "Not moduloc correct");
								for (size_t iIndice = 0; iIndice < accessor.count; iIndice += 3)
								{
									memcpy(&data, &buffer.data[bufferView.byteOffset + accessor.byteOffset + iIndice], 4);
									newPrim.triangles.emplace_back(data[0], data[1], data[2]);
								}
							}
							break;
							case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
							{
								unsigned int data[3];
								ASSERT(accessor.count % 3 == 0, "Not moduloc correct");
								for (size_t iIndice = 0; iIndice < accessor.count; iIndice += 3)
								{
									memcpy(&data, &buffer.data[bufferView.byteOffset + accessor.byteOffset + iIndice], 4);
									newPrim.triangles.emplace_back(data[0], data[1], data[2]);
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
						// POSITION, NORMAL, TEXCOORD_0, TEXCOORD_1, COLOR_0
						{ // POSITION
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
						{ // NORMAL
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
						{ // TANGENT
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
						{ // TEXCOORD_0
							std::map<std::string, int>::iterator it = tinyPrimitive.attributes.find("TEXCOORD_0");
							if (it != tinyPrimitive.attributes.end())
							{
								tinygltf::Accessor &accessor = tinyModel.accessors[it->second];
								tinygltf::BufferView &bufferView = tinyModel.bufferViews[accessor.bufferView];
								tinygltf::Buffer &buffer = tinyModel.buffers[bufferView.buffer];
								size_t size = accessor.count * tinygltf::GetTypeSizeInBytes(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
								ASSERT(accessor.type == TINYGLTF_TYPE_VEC4, "TEXCOORD_0 not vec2");
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
						{ // TEXCOORD_1
							std::map<std::string, int>::iterator it = tinyPrimitive.attributes.find("TEXCOORD_0");
							if (it != tinyPrimitive.attributes.end())
							{
								tinygltf::Accessor &accessor = tinyModel.accessors[it->second];
								tinygltf::BufferView &bufferView = tinyModel.bufferViews[accessor.bufferView];
								tinygltf::Buffer &buffer = tinyModel.buffers[bufferView.buffer];
								size_t size = accessor.count * tinygltf::GetTypeSizeInBytes(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
								ASSERT(accessor.type == TINYGLTF_TYPE_VEC4, "TEXCOORD_0 not vec2");
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
						{ // COLOR_0
							std::map<std::string, int>::iterator it = tinyPrimitive.attributes.find("TEXCOORD_0");
							if (it != tinyPrimitive.attributes.end())
							{
								tinygltf::Accessor &accessor = tinyModel.accessors[it->second];
								tinygltf::BufferView &bufferView = tinyModel.bufferViews[accessor.bufferView];
								tinygltf::Buffer &buffer = tinyModel.buffers[bufferView.buffer];
								size_t size = accessor.count * tinygltf::GetTypeSizeInBytes(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
								ASSERT(accessor.type == TINYGLTF_TYPE_VEC4, "TEXCOORD_0 not vec2");
								switch (accessor.type)
								{
								case TINYGLTF_TYPE_VEC3:
								{
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
								}
								break;
								case TINYGLTF_TYPE_VEC4:
								{
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
				}
				scene.primitives.push_back(newPrim);
			}

			//nodes.reserve(tinyModel.nodes.size());
			/*m_scene->nodes.reserve(tinyModel.nodes.size());
			for (size_t iNode = 0; iNode < tinyModel.nodes.size(); iNode++)
			{
				tinygltf::Node &tinyNode = tinyModel.nodes[iNode];
				gltf::Node &node = m_scene->addNode();
				node.index = static_cast<int>(iNode);
				for (size_t iChild = 0; iChild < tinyNode.children.size(); iChild++)
				{
					const int childID = tinyNode.children[iChild];
					node.addChildren(&m_scene->nodes[childID]);
				}
				if (tinyNode.mesh != -1)
					node.mesh = &m_scene->meshes[tinyNode.mesh];

				if (tinyNode.matrix.size() == 16)
				{
					int globalIndex = 0;
					for (int col = 0; col < 4; col++)
						for (int row = 0; row < 4; row++)
							node.localTransform[col][row] = static_cast<float>(tinyNode.matrix[globalIndex++]);
				}
				else
				{
					Vector3f translation(0.f);
					Quaternionf rotation = Quaternionf::identity();
					Vector3f scale(1.f);

					if (tinyNode.translation.size() == 3)
						for (int i = 0; i < 3; i++)
							translation[i] = static_cast<float>(tinyNode.translation[i]);

					if (tinyNode.scale.size() == 3)
						for (int i = 0; i < 3; i++)
							scale[i] = static_cast<float>(tinyNode.scale[i]);

					if (tinyNode.rotation.size() == 4)
						for (int i = 0; i < 4; i++)
							rotation[i] = static_cast<float>(tinyNode.rotation[i]);

					node.localTransform = Matrix4f::TRS(translation, rotation, scale);
				}
			}*/
			return scene;
		}
		bool Scene::GLTF::write(const Scene & scene)
		{
			return false;
		}
	}
}

