#include "Loader.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "tiny_gltf.h"
#include <fstream>

namespace raycore {

	namespace GLTF {
		bool load(std::string path, Scene &scene)
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
			const size_t nbScene = tinyModel.scenes.size();
			// TODO manage multiple scenes
			// --- TEXTURES
			scene.textures.reserve(tinyModel.textures.size());
			for (size_t iTex = 0; iTex < tinyModel.textures.size(); iTex++)
			{
				tinygltf::Texture &tinyTex = tinyModel.textures[iTex];
				tinygltf::Image &tinyImage = tinyModel.images[tinyTex.source];
				Texture<colorHDR> *newTex = scene.addTexture(new TextureMap32(tinyImage.image, tinyImage.width, tinyImage.height, tinyImage.component));
			}
			// --- MATERIALS
			scene.materials.reserve(tinyModel.materials.size());
			for (size_t iMaterial = 0; iMaterial < tinyModel.materials.size(); iMaterial++)
			{
				tinygltf::Material &tinyMat = tinyModel.materials[iMaterial];
				Material &newMat = scene.addMaterial();
				newMat.index = static_cast<unsigned int>(iMaterial);
				auto itColor = tinyMat.values.find("baseColorFactor");
				if (itColor == tinyMat.values.end())
					newMat.color = colorHDR(1.f);
				else
				{
					tinygltf::ColorValue c = itColor->second.ColorFactor();
					newMat.color = colorHDR(
						static_cast<float>(c[0]),
						static_cast<float>(c[1]),
						static_cast<float>(c[2]),
						static_cast<float>(c[3])
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
					newMat.texture = scene.textures[index];
				}
			}
			// --- MESHES
			tinygltf::Buffer &buffer = tinyModel.buffers[0];
			/*size_t sizePrim = 0;
			for (size_t iMesh = 0; iMesh < tinyModel.meshes.size(); iMesh++)
				sizePrim += tinyModel.meshes[iMesh].primitives.size();
			scene.primitives.reserve(sizePrim);*/
			for (size_t iMesh = 0; iMesh < tinyModel.meshes.size(); iMesh++)
			{
				tinygltf::Mesh &tinyMesh = tinyModel.meshes[iMesh];
				Mesh &newMesh = scene.addMesh();
				for (size_t iPrimitive = 0; iPrimitive < tinyMesh.primitives.size(); iPrimitive++)
				{
					tinygltf::Primitive &tinyPrimitive = tinyMesh.primitives[iPrimitive];
					newMesh.primitives.emplace_back();
					Primitive &newPrim = newMesh.primitives.back();// scene.addPrimitive();
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
											Vertex &vert = newPrim.vertices[iVert];
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
								for (size_t iVert = 0; iVert < newPrim.vertices.size(); iVert++)
								{
									Vertex &vert = newPrim.vertices[iVert];
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
					//newMesh.primitives.push_back(&newPrim);
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
			return true;
		}
		bool write(std::string path, const Scene & scene)
		{
			return false;
		}
	}

	namespace OBJ {
		bool load(std::string path, Mesh & mesh)
		{
			std::ifstream file(path);
			std::string line;

			mesh.primitives.emplace_back();
			Primitive &prim = mesh.primitives.back();
			
			std::vector<point3> positions;
			std::vector<norm3> normals;
			std::vector<uv2> uvs;
			std::vector<Face> faces;
			std::vector<Vertex> vertices;
			while (std::getline(file, line))
			{
				std::stringstream ss(line);
				std::string header;
				ss >> header;
				switch (header[0])
				{
				case 'v':
				case 'V': {
					switch (line[1])
					{
					case 'n':
					case 'N': {
						norm3 norm;
						ss >> norm.x >> norm.y >> norm.z;
						normals.push_back(norm);
						break;
					}
					case 't':
					case 'T': {
						uv2 uv;
						ss >> uv.u >> uv.v;
						uvs.push_back(uv);
						break;
					}
					case ' ': {
						point3 pos;
						ss >> pos.x >> pos.y >> pos.z;
						positions.push_back(pos);
						break;
					}
					default:
						break;
					}
					break;
				}
				case 'f':
				case 'F': {
					Face face;
					for(int iVert = 0; iVert < 3; iVert++)
					{
						ss >> face.posID.vertices[iVert];
						if (ss.peek() == '/')
						{
							ss.ignore(1);
							if (ss.peek() == '/')
							{
								ss.ignore(1);
								ss >> face.normID.vertices[iVert];
							}
							else
							{
								ss >> face.uvID.vertices[iVert];
								if (ss.peek() == '/')
									ss >> face.normID.vertices[iVert];
							}
						}
					}
					faces.push_back(face);
					break;
				}
				case '#':
					Log::warn("Skipping comment");
					break;
				case 'o':
					Log::warn("Data not implemented : ", line);
					break;
				default:
					Log::error("Unknown data : ", line);
					break;
				}
			}
			for (size_t iTri = 0; iTri < faces.size(); iTri++)
			{
				Face &face = faces[iTri];
				vec3 AB(positions[face.posID.B - 1] - positions[face.posID.A - 1]);
				vec3 AC(positions[face.posID.C - 1] - positions[face.posID.A - 1]);
				norm3 normal(normalize(cross(AB, AC)));
				for (size_t iVert = 0; iVert < 3; iVert++)
				{
					Vertex vert;
					vert.position = positions[face.posID.vertices[iVert] - 1];
					if (normals.size() > 0)
						vert.normal = normals[face.normID.vertices[iVert] - 1];
					else
						vert.normal = normal;
					if(uvs.size() > 0)
						vert.texcoord = uvs[face.uvID.vertices[iVert] - 1];
					else
						vert.texcoord = uv2(0.f);
					vert.color = colorHDR(1.f);
					prim.vertices.push_back(vert);
				}
				prim.triangles.push_back(Triangle(iTri * 3, iTri * 3 + 1, iTri * 3 + 2));
			}
			return true;
		}

		bool write(std::string path, const Mesh & scene)
		{
			return false;
		}
	}

	namespace builder {

		bool build(Scene &scene)
		{
			scene.textures.reserve(2);
			scene.materials.reserve(5);
			scene.shapes.reserve(5);
			scene.nodes.reserve(5);

			// materials
			Material &mat1 = scene.addMaterial();
			mat1.index = 0;
			mat1.color = colorHDR(0.9f);
			mat1.texture = nullptr;
			mat1.type = MaterialType::DIELECTRIC;

			Material &mat2 = scene.addMaterial();
			mat2.index = 1;
			mat2.color = colorHDR(0.5f, 1.f, 0.5f, 1.f);
			mat2.texture = nullptr;
			mat2.type = MaterialType::DIFFUSE;

			Material &mat3 = scene.addMaterial();
			mat3.index = 2;
			mat3.color = colorHDR(0.82f, 0.62f, 0.19f, 1.f);
			mat3.texture = nullptr;
			mat3.type = MaterialType::METAL;

			Texture<colorHDR> *texture1 = scene.addTexture(new Checker<colorHDR>(colorHDR(0.1f), colorHDR(1.f), 40, 40));
			Material &mat4 = scene.addMaterial();
			mat4.index = 3;
			mat4.color = colorHDR(0.7f, 0.7f, 0.7f, 1.f);
			mat4.texture = texture1;
			mat4.type = MaterialType::DIFFUSE;

			Texture<colorHDR> *texture2 = scene.addTexture(new Checker<colorHDR>(colorHDR(0.5f, 0.5f, 1.f, 1.f), colorHDR(1.f, 0.5f, 0.5f, 1.f), 1, 1));
			Material &mat5 = scene.addMaterial();
			mat5.index = 4;
			mat5.color = colorHDR(0.5f, 0.8f, 0.3f, 1.f);
			mat5.texture = texture2;
			mat5.type = MaterialType::DIFFUSE;

			// spheres
			Sphere &sphere1 = scene.addSphere();
			sphere1.center = point3(0.f);
			sphere1.radius = 0.4f;
			sphere1.up = norm3(0.f, 1.f, 0.f);
			sphere1.material = &mat1;

			Sphere &sphere2 = scene.addSphere();
			sphere2.center = point3(0.f, 0.1f, -1.0f);
			sphere2.radius = 0.5f;
			sphere2.up = norm3(0.f, 1.f, 0.f);
			sphere2.material = &mat2;

			Sphere &sphere3 = scene.addSphere();
			sphere3.center = point3(1.5f, 0.2f, -0.5f);
			sphere3.radius = 0.7f;
			sphere3.up = norm3(0.f, 1.f, 0.f);
			sphere3.material = &mat3;

			Sphere &sphere4 = scene.addSphere();
			sphere4.center = point3(0.f, -30.f, 0.f);
			sphere4.radius = 29.f;
			sphere4.up = norm3(1.f, 0.f, 0.f);
			sphere4.material = &mat4;

			Sphere &sphere5 = scene.addSphere();
			sphere5.center = point3(-1.5f, 0.2f, 0.f);
			sphere5.radius = 0.6f;
			sphere5.up = normalize(norm3(1.f));
			sphere5.material = &mat5;

			Mesh &mesh = scene.addMesh();
			raycore::OBJ::load("../data/models/dragon/dragonn.obj", mesh);
			mesh.primitives[0].material = &mat1;

			// nodes
			Node &node1 = scene.addNode();
			node1.shape = &sphere1;
			node1.transform = mat4::identity();

			Node &node2 = scene.addNode();
			node2.shape = &sphere2;
			node2.transform = mat4::identity();

			Node &node3 = scene.addNode();
			node3.shape = &sphere3;
			node3.transform = mat4::identity();

			Node &node4 = scene.addNode();
			node4.shape = &sphere4;
			node4.transform = mat4::identity();

			Node &node5 = scene.addNode();
			node5.shape = &sphere5;
			node5.transform = mat4::identity();

			Node &node6 = scene.addNode();
			node6.shape = &mesh;
			node6.transform = mat4::identity();
			node6.transform[0].x = 1.f;
			node6.transform[1].y = 1.f;
			node6.transform[2].z = 1.f;
			node6.transform[3].x = 0.f;
			node6.transform[3].y = 0.f;
			node6.transform[3].z = -2.f;

			return true;
		}

	}
}