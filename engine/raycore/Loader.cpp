#include "Loader.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "tiny_gltf.h"
#include <fstream>

#include "Triangle.h"
#include "Sphere.h"
#include "PointLight.h"
#include "ShapeLight.h"

namespace raycore {

	namespace loader {
		Primitive::Primitive()
		{

		}
		Primitive::Primitive(const loader::Mesh & mesh, prim::Material * material)
		{
			for (const loader::Mesh::Face &face : mesh.faces)
			{
				prim::Vertex triVert[3];
				for (size_t iVert = 0; iVert < 3; iVert++)
				{
					triVert[iVert] = prim::Vertex(
						mesh.vertices[face.data[iVert]].position,
						mesh.vertices[face.data[iVert]].normal,
						mesh.vertices[face.data[iVert]].uv,
						mesh.vertices[face.data[iVert]].color
					);
				}
				prim::Triangle *tri = new prim::Triangle(
					triVert[0],
					triVert[1],
					triVert[2]
				);
				tri->material = material;
				this->hitables.push_back(tri);
			}
		}

		Primitive::Primitive(const loader::Sphere & sphere, prim::Material * material)
		{
			prim::Sphere *hitableSphere = new prim::Sphere(sphere.center, sphere.radius, sphere.up);
			hitableSphere->material = material;
			this->hitables.push_back(hitableSphere);
		}
		Primitive::~Primitive()
		{
			/*for (prim::Hitable *hitable : hitables)
				delete hitable;*/
		}
	}

	namespace GLTF {
		bool load(std::string path, prim::Scene &scene)
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
			std::vector<loader::Primitive> prims(tinyModel.meshes.size());
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

			for (size_t iMesh = 0; iMesh < tinyModel.meshes.size(); iMesh++)
			{
				tinygltf::Mesh &tinyMesh = tinyModel.meshes[iMesh];
				for (size_t iPrimitive = 0; iPrimitive < tinyMesh.primitives.size(); iPrimitive++)
				{
					tinygltf::Primitive &tinyPrimitive = tinyMesh.primitives[iPrimitive];
					loader::Mesh newMesh;

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
								newMesh.vertices.reserve(accessor.count);
								for (size_t iVert = 0; iVert < accessor.count; iVert++)
								{
									newMesh.vertices.emplace_back();
									loader::Mesh::Vertex &vert = newMesh.vertices.back();
									memcpy(
										vert.position.data,
										&buffer.data[bufferView.byteOffset + accessor.byteOffset + iVert * accessor.ByteStride(bufferView)],
										sizeof(point3)
									);
									vert.position = coordinates(vert.position);
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
									loader::Mesh::Vertex &vert = newMesh.vertices[iVert];
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
										loader::Mesh::Vertex &vert = newMesh.vertices[iVert];
										memcpy(
											vert.uv.data,
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
											loader::Mesh::Vertex &vert = newMesh.vertices[iVert];
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
											loader::Mesh::Vertex &vert = newMesh.vertices[iVert];
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
								for (size_t iVert = 0; iVert < newMesh.vertices.size(); iVert++)
								{
									loader::Mesh::Vertex &vert = newMesh.vertices[iVert];
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
									newMesh.faces.push_back(loader::Mesh::Face(data[0], data[1], data[2]));
									ASSERT(static_cast<size_t>(data[0]) < newMesh.vertices.size(), "what");
									ASSERT(static_cast<size_t>(data[1]) < newMesh.vertices.size(), "what");
									ASSERT(static_cast<size_t>(data[2]) < newMesh.vertices.size(), "what");
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
									newMesh.faces.push_back(loader::Mesh::Face(data[0], data[1], data[2]));
									ASSERT(static_cast<size_t>(data[0]) < newMesh.vertices.size(), "what");
									ASSERT(static_cast<size_t>(data[1]) < newMesh.vertices.size(), "what");
									ASSERT(static_cast<size_t>(data[2]) < newMesh.vertices.size(), "what");
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
									newMesh.faces.push_back(loader::Mesh::Face(data[0], data[1], data[2]));
									ASSERT(static_cast<size_t>(data[0]) < newMesh.vertices.size(), "what");
									ASSERT(static_cast<size_t>(data[1]) < newMesh.vertices.size(), "what");
									ASSERT(static_cast<size_t>(data[2]) < newMesh.vertices.size(), "what");
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
									newMesh.faces.push_back(loader::Mesh::Face(data[0], data[1], data[2]));
									ASSERT(static_cast<size_t>(data[0]) < newMesh.vertices.size(), "what");
									ASSERT(static_cast<size_t>(data[1]) < newMesh.vertices.size(), "what");
									ASSERT(static_cast<size_t>(data[2]) < newMesh.vertices.size(), "what");
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
									newMesh.faces.push_back(loader::Mesh::Face(data[0], data[1], data[2]));
									ASSERT(static_cast<size_t>(data[0]) < newMesh.vertices.size(), "what");
									ASSERT(static_cast<size_t>(data[1]) < newMesh.vertices.size(), "what");
									ASSERT(static_cast<size_t>(data[2]) < newMesh.vertices.size(), "what");
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
									newMesh.faces.push_back(loader::Mesh::Face(data[0], data[1], data[2]));
									ASSERT(static_cast<size_t>(data[0]) < newMesh.vertices.size(), "what");
									ASSERT(static_cast<size_t>(data[1]) < newMesh.vertices.size(), "what");
									ASSERT(static_cast<size_t>(data[2]) < newMesh.vertices.size(), "what");
								}
							}
							}
							break;
						default:
							throw std::runtime_error("TINYGLTF Error : Accessor type not supported");
						}
					}
					prims[iMesh] = loader::Primitive(newMesh, materials[tinyPrimitive.material]);
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
					node = new prim::Geometry(prims[tinyNode.mesh].hitables);
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
						if(nodes[childID] != nullptr)
							group->addChild(nodes[childID]);
					}
				}
			}
			prim::Group* root = dynamic_cast<prim::Group*>(nodes[tinyModel.scenes[0].nodes[0]]);
			if (root == nullptr)
			{
				root = new prim::Group();
				for(size_t iNode = 0; iNode < tinyModel.scenes[tinyModel.defaultScene].nodes.size(); iNode++)
					root->addChild(nodes[tinyModel.scenes[tinyModel.defaultScene].nodes[iNode]]);
			}
			scene.setRoot(root);
			scene.setLightDistribution(new prim::LightDistribution);
			return true;
		}
		bool write(std::string path, const prim::Scene & scene)
		{
			return false;
		}
	}

	namespace OBJ {
		bool load(std::string path, loader::Mesh & mesh)
		{
			std::ifstream file(path);
			if (!file)
				throw std::runtime_error("Could not open file " + path);
			std::string line;

			std::vector<point3> positions;
			std::vector<norm3> normals;
			std::vector<uv2> uvs;
			std::vector<FaceID> faces;
			std::vector<loader::Mesh::Vertex> vertices;

			auto coordinates = [](const point3 &p) -> point3
			{
				return point3(p.x, -p.z, p.y);
			};
			auto coordinatesN = [](const norm3 &p) -> norm3
			{
				return norm3(p.x, -p.z, p.y);
			};

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
					FaceID face;
					for (int iVert = 0; iVert < 3; iVert++)
					{
						ss >> face.posID.data[iVert];
						if (ss.peek() == '/')
						{
							ss.ignore(1);
							if (ss.peek() == '/')
							{
								ss.ignore(1);
								ss >> face.normID.data[iVert];
							}
							else
							{
								ss >> face.uvID.data[iVert];
								if (ss.peek() == '/')
									ss >> face.normID.data[iVert];
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
				FaceID &face = faces[iTri];
				vec3 AB(positions[face.posID.data[1] - 1] - positions[face.posID.data[0] - 1]);
				vec3 AC(positions[face.posID.data[2] - 1] - positions[face.posID.data[0] - 1]);
				norm3 normal(normalize(cross(AB, AC)));
				for (size_t iVert = 0; iVert < 3; iVert++)
				{
					loader::Mesh::Vertex vert;
					vert.position = coordinates(positions[face.posID.data[iVert] - 1]);
					if (normals.size() > 0)
						vert.normal = coordinatesN(normals[face.normID.data[iVert] - 1]);
					else
						vert.normal = coordinatesN(normal);
					if (uvs.size() > 0)
						vert.uv = uvs[face.uvID.data[iVert] - 1];
					else
						vert.uv = uv2(0.f);
					vert.color = colorHDR(1.f);
					mesh.vertices.push_back(vert);
				}
				mesh.faces.push_back(loader::Mesh::Face(iTri * 3, iTri * 3 + 1, iTri * 3 + 2));
			}
			return true;
		}

		bool write(std::string path, const loader::Mesh & scene)
		{
			return false;
		}
	}

	namespace builder {

		bool build(prim::Scene & scene)
		{
			prim::Group *root = new prim::Group();
			root->setAcceleration(prim::Acceleration::BVH);

			Texture<colorHDR> *tex0 = scene.addTexture(new ConstantTexture<colorHDR>(colorHDR(0.9f)));
			Texture<colorHDR> *tex1 = scene.addTexture(new ConstantTexture<colorHDR>(colorHDR(0.5f, 1.f, 0.5f, 1.f)));
			Texture<colorHDR> *tex2 = scene.addTexture(new ConstantTexture<colorHDR>(colorHDR(0.82f, 0.62f, 0.19f, 1.f)));
			Texture<colorHDR> *tex3 = scene.addTexture(new Checker<colorHDR>(colorHDR(0.1f), colorHDR(1.f), 40, 40));
			Texture<colorHDR> *tex4 = scene.addTexture(new Checker<colorHDR>(colorHDR(0.5f, 0.5f, 1.f, 1.f), colorHDR(1.f, 0.5f, 0.5f, 1.f), 2, 2));
			Texture<float> *roughness = new ConstantTexture<float>(0.1f); // TODO leak HERE

			prim::Material *mat00 = scene.addMaterial(new prim::Glass(tex0, 1.1f));
			prim::Material *mat01 = scene.addMaterial(new prim::Matte(tex1));
			prim::Material *mat02 = scene.addMaterial(new prim::Metal(tex2, roughness));
			prim::Material *mat03 = scene.addMaterial(new prim::Matte(tex3));
			prim::Material *mat04 = scene.addMaterial(new prim::Matte(tex4));


			loader::Sphere sphere0;
			sphere0.center = point3(0.f);
			sphere0.radius = 0.4f;
			sphere0.up = norm3(settings::up);
			loader::Primitive prim0(sphere0, mat00);
			prim::Geometry *geom0 = new prim::Geometry(prim0.hitables);
			geom0->setAcceleration(prim::Acceleration::NONE);
			root->addChild(geom0);

			loader::Sphere sphere1;
			sphere1.center = point3(0.f, -1.0f, 0.1f);
			sphere1.radius = 0.5f;
			sphere1.up = norm3(settings::up);
			loader::Primitive prim1(sphere1, mat01);
			prim::Geometry *geom1 = new prim::Geometry(prim1.hitables);
			geom1->setAcceleration(prim::Acceleration::NONE);
			root->addChild(geom1);

			loader::Sphere sphere2;
			sphere2.center = point3(1.5f, -0.5f, 0.2f);
			sphere2.radius = 0.7f;
			sphere2.up = norm3(settings::up);
			loader::Primitive prim2(sphere2, mat02);
			prim::Geometry *geom2 = new prim::Geometry(prim2.hitables);
			geom2->setAcceleration(prim::Acceleration::NONE);
			root->addChild(geom2);

			loader::Sphere sphere3;
			sphere3.center = point3(0.f, 0.f, -30.f);
			sphere3.radius = 29.f;
			sphere3.up = norm3(settings::right);
			loader::Primitive prim3(sphere3, mat03);
			prim::Geometry *geom3 = new prim::Geometry(prim3.hitables);
			geom3->setAcceleration(prim::Acceleration::NONE);
			root->addChild(geom3);

			loader::Sphere sphere4;
			sphere4.center = point3(-1.5f, 0.f, 0.2f);
			sphere4.radius = 0.6f;
			sphere4.up = normalize(norm3(1.f));
			loader::Primitive prim4(sphere4, mat04);
			prim::Geometry *geom4 = new prim::Geometry(prim4.hitables);
			geom4->setAcceleration(prim::Acceleration::NONE);
			root->addChild(geom4);

			loader::Mesh mesh5;
			raycore::OBJ::load("../raycore/data/models/bunny/bunny.obj", mesh5);
			//mesh5.materialID = 0;
			loader::Primitive prim5(mesh5, mat02);
			prim::Geometry *geom5 = new prim::Geometry(prim5.hitables);
			geom5->setAcceleration(prim::Acceleration::BVH);
			//geom5->setTransform(mat4(
			//	mat4::col(15.f,  0.f,  0.f, 0.f),
			//	mat4::col( 0.f, 15.f,  0.f, 0.f),
			//	mat4::col( 0.f,  0.f, 15.f, 2.f),
			//	mat4::col( 0.f,  0.f,  0.f, 1.f)
			//));
			root->addChild(geom5);

			prim::LightDistribution *ld = new prim::LightDistribution();
			//ld->addLight(new prim::ShapeLight(new prim::shape::Sphere(point3(0.f, 5.f, 0.f), 1.f), colorHDR(1.f), 1.f));
			//ld->addLight(new prim::ShapeLight(new prim::shape::Disk(point3(5.f, 2.f, 0.f), 1.f, norm3(0.f, 1.f, 0.f)), colorHDR(1.f), 1.f));
			//ld->addLight(new prim::PointLight(point3(5.f, 5.f, 0.f), colorHDR(1.f), 1.f));
			scene.setLightDistribution(ld);
			scene.setRoot(root);
			return true;
		}

	}
}