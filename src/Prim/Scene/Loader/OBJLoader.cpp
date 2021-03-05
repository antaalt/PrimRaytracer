#include "OBJLoader.h"

#include <string>
#include <sstream>
#include <array>
#include <map>
#include <fstream>
#include <memory>

#include <Aka/OS/Logger.h>
#include <Aka/OS/Image.h>
#include <utf8.h>

#include "../Hitable/MeshBVH.h"
#include "../Hitable/TransformNode.h"
#include "../Texture/ConstantTexture.h"
#include "../Texture/ImageTexture.h"
#include "../Material/Matte.h"

namespace prim {

namespace obj {

struct Vertex {
	uint32_t posID;
	uint32_t normID;
	uint32_t uvID;
};

struct Face {
	std::vector<Vertex> vertices;
	Vertex &operator[](size_t iVert) { return vertices[iVert]; }
	const Vertex &operator[](size_t iVert) const { return vertices[iVert]; }
};

// https://www.fileformat.info/format/material/index.htm
struct Material {
	std::string name;
	vec3f Ka; // ambiant color
	vec3f Kd; // diffuse color
	vec3f Ks; // specular color
	vec3f Ke; // emissive color
	float Ni; // optic density
	float Ns; // specular exponent
	float d; // transparency [0, 1], 1 = no transparency
	unsigned int illum; // light parameters
	std::string map_Kd;
	std::string map_Ka;
	std::string map_bump;
};

struct Group {
	std::string name;
	std::vector<Face> faces;
	std::vector<prim::Material*> materials;
};

struct Object {
	std::string name;
	std::vector<Group> groups;
};

}

void skipWhitespace(std::stringstream &ss)
{
	char whitespaces[] = { ' ', '\t' };
	do
	{
		int c = ss.peek();
		for (char &whitespace : whitespaces)
		{
			if (c == whitespace)
			{
				ss.ignore(1);
				continue;
			}
		}
		break;
	} while (true);
}

void parseMaterials(const aka::Path& fileName, std::map<std::string, std::unique_ptr<obj::Material>> &materials) {
#if defined(AKA_PLATFORM_WINDOWS)
	std::wstring wstr;
	utf8::utf8to16(fileName.str().begin(), fileName.str().end(), std::back_inserter(wstr));
	std::ifstream file(wstr.c_str());
#else
	std::ifstream file(fileName.c_str());
#endif
	if (!file)
		throw std::runtime_error("Could not load file " + fileName.str());

	obj::Material *currentMaterial = nullptr;

	std::string line;
	while (std::getline(file, line))
	{
		if (file.eof())
			break;
		if (line.size() == 0)
			continue;
		std::stringstream ss(line);
		std::string header;
		ss >> header;

		if (header == "newmtl")
		{
			std::string materialName;
			ss >> materialName;
			std::unique_ptr<obj::Material> material = std::make_unique<obj::Material>();
			currentMaterial = material.get();
			currentMaterial->name = materialName;
			materials.insert(std::make_pair(materialName, std::move(material)));
		}
		else 
		{
			if (header == "map_Kd") {
				std::string texturePath;
				ss >> texturePath;
				currentMaterial->map_Kd = texturePath;
			} else if (header == "map_Ka") {
				std::string texturePath;
				ss >> texturePath;
				currentMaterial->map_Ka = texturePath;
			} else if (header == "map_bump") {
				std::string texturePath;
				ss >> texturePath;
				currentMaterial->map_bump = texturePath;
			} else if (header == "Ka") {
				ss >> currentMaterial->Ka.x;
				ss >> currentMaterial->Ka.y;
				ss >> currentMaterial->Ka.z;
			} else if (header == "Kd") {
				ss >> currentMaterial->Kd.x;
				ss >> currentMaterial->Kd.y;
				ss >> currentMaterial->Kd.z;
			} else if (header == "Ks") {
				ss >> currentMaterial->Ks.x;
				ss >> currentMaterial->Ks.y;
				ss >> currentMaterial->Ks.z;
			} else if (header == "Ke") {
				ss >> currentMaterial->Ke.x;
				ss >> currentMaterial->Ke.y;
				ss >> currentMaterial->Ke.z;
			} else if (header == "Ni") {
				ss >> currentMaterial->Ni;
			} else if (header == "Ns") {
				ss >> currentMaterial->Ns;
			} else if (header == "d") {
				ss >> currentMaterial->d;
			} else if (header == "illum") {
				ss >> currentMaterial->illum;
			}
		}
	}
}

Material *convert(const aka::Path &path, Scene &scene, obj::Material *objMaterial)
{
	// TODO use others parameters.
	Texture4f *texture;
	if (objMaterial->map_Kd.length() > 0)
	{
		aka::ImageHDR image = aka::ImageHDR::load(path + objMaterial->map_Kd);
		texture = new ImageTexture4f((color4f*)image.bytes.data(), image.width, image.height);
	}
	else if (objMaterial->map_Ka.length() > 0)
	{
		aka::ImageHDR image = aka::ImageHDR::load(path + objMaterial->map_Ka);
		texture = new ImageTexture4f((color4f*)image.bytes.data(), image.width, image.height);
	}
	else 
		texture = new ConstantTexture4f(color4f(objMaterial->Kd, 1.f));
	Material *material = new Matte(texture);
	scene.textures.push_back(texture);
	scene.materials.push_back(material);
	return material;
}

bool OBJLoader::load(const aka::Path &fileName, Scene &scene)
{
	std::vector<point3f> positions;
	std::vector<norm3f> normals;
	std::vector<uv2f> uvs;
	std::vector<obj::Object> objects;
	std::map<std::string, Material*> materials;
	Material *currentMaterial = nullptr;

	aka::Path path = fileName.up();
#if defined(AKA_PLATFORM_WINDOWS)
	std::wstring wstr;
	utf8::utf8to16(fileName.begin(), fileName.end(), std::back_inserter(wstr));
	std::ifstream file(wstr.c_str());
#else
	std::ifstream file(fileName.c_str());
#endif
	if (!file)
		throw std::runtime_error("Could not load file " + fileName.str());

	std::string line;
	while (std::getline(file, line))
	{
		if (file.eof())
			break;
		if (line.size() == 0)
			continue;
		std::stringstream ss(line);
		std::string header;
		ss >> header;
		if (header == "mtllib")
		{
			std::string materialFileName;
			ss >> materialFileName;
			std::map<std::string, std::unique_ptr<obj::Material>> parsedMaterials;
			parseMaterials(path + materialFileName, parsedMaterials);
			// convert obj::materials to materials.
			for (auto const& it : parsedMaterials) {
				Material *material = convert(path, scene, it.second.get());
				materials.insert(std::make_pair(it.first, material));
			}
			continue;
		}
		else if (header == "usemtl")
		{
			std::string materialName;
			ss >> materialName; 
			auto it = materials.find(materialName);
			if (it == materials.end())
				throw std::runtime_error("Material not found : " + materialName);
			currentMaterial = it->second;
			continue;
		}
		switch (header[0])
		{
		case 'v':
		case 'V': {
			switch (line[1])
			{
			case 'n':
			case 'N': {
				norm3f norm;
				ss >> norm.x >> norm.y >> norm.z;
				normals.push_back(norm);
				break;
			}
			case 't':
			case 'T': {
				uv2f uv;
				ss >> uv.u >> uv.v;
				uvs.push_back(uv);
				break;
			}
			case ' ': {
				point3f pos;
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
			// If no object, create default one.
			if (objects.size() == 0)
			{
				objects.emplace_back();
				objects.back().name = "default";
			}
			// If no group, create default one.
			if (objects.back().groups.size() == 0)
			{
				objects.back().groups.emplace_back();
				objects.back().groups.back().name = "default";
			}
			obj::Group &group = objects.back().groups.back();
			group.faces.emplace_back();
			if (currentMaterial != nullptr)
			{
				group.materials.push_back(currentMaterial);
			}
			else
			{
				scene.textures.push_back(new ConstantTexture4f(color4f(0.8f, 0.8f, 0.8f, 1.f)));
				currentMaterial = new Matte(scene.textures.back());
				scene.materials.push_back(currentMaterial);
				group.materials.push_back(currentMaterial);
			}
			obj::Face &face = group.faces.back();
			while (ss.peek() != std::char_traits<char>::eof())
			{
				face.vertices.emplace_back();
				obj::Vertex &vertex = face.vertices.back();
				ss >> vertex.posID;
				if (ss.peek() == '/')
				{
					ss.ignore(1);
					if (ss.peek() == '/')
					{
						ss.ignore(1);
						ss >> vertex.normID;
					}
					else
					{
						ss >> vertex.uvID;
						if (ss.peek() == '/')
						{
							ss.ignore(1);
							ss >> vertex.normID;
						}
					}
				}
				skipWhitespace(ss);
			}
			break;
		}
		case '#':
			break;
		case 'O':
		case 'o': { // New object
			std::string objectName;
			ss >> objectName;
			objects.emplace_back();
			objects.back().name = objectName;
			break;
		}
		case 'S':
		case 's': { // Smoothing group
			break;
		}
		case 'G':
		case 'g': { // New face group
			std::string groupName;
			ss >> groupName;
			if (objects.size() == 0)
			{
				objects.emplace_back();
				objects.back().name = "default";
			}
			objects.back().groups.emplace_back();
			objects.back().groups.back().name = groupName;
			break;
		}
		default:
			aka::Logger::warn("Unknown data : ", line);
			break;
		}
	}
	size_t totalTriCount = 0;
	for (const obj::Object &object : objects)
	{
		MeshBVH *meshBVH = new MeshBVH(nullptr);
		TransformNode *node = new TransformNode(mat4f::identity(), meshBVH);
		scene.nodes.push_back(node);
		uint32_t iVert = 0;
		for (const obj::Group &group : object.groups)
		{
			for (size_t iFace = 0; iFace < group.faces.size(); iFace++)
			{
				const obj::Face &face = group.faces[iFace];
				Material *material = group.materials[iFace];
				if (face.vertices.size() == 3)
				{
					Triangle triangle(material);
					// It's a triangle !
					vec3f AB(positions[face[1].posID - 1] - positions[face[0].posID - 1]);
					vec3f AC(positions[face[2].posID - 1] - positions[face[0].posID - 1]);
					norm3f normal(vec3f::normalize(vec3f::cross(AB, AC)));
					for (size_t iVert = 0; iVert < 3; iVert++)
					{
						triangle.vertices[iVert].position = positions[face[iVert].posID - 1];
						if (normals.size() > 0)
							triangle.vertices[iVert].normal = normals[face[iVert].normID - 1];
						else
							triangle.vertices[iVert].normal = normal;
						if (uvs.size() > 0)
							triangle.vertices[iVert].texcoord = uvs[face[iVert].uvID - 1];
						else
							triangle.vertices[iVert].texcoord = uv2f(0.f);
					}
					iVert += 3;
					totalTriCount += 1;
					meshBVH->addTriangle(triangle);
				}
				else if (face.vertices.size() == 4)
				{
					// It's a quad !
					std::array<Triangle::Vertex, 4> vertices;
					vec3f AB(positions[face[1].posID - 1] - positions[face[0].posID - 1]);
					vec3f AC(positions[face[2].posID - 1] - positions[face[0].posID - 1]);
					norm3f normal(vec3f::normalize(vec3f::cross(AB, AC)));
					for (size_t iVert = 0; iVert < 4; iVert++)
					{
						vertices[iVert].position = positions[face[iVert].posID - 1];
						if (normals.size() > 0)
							vertices[iVert].normal = normals[face[iVert].normID - 1];
						else
							vertices[iVert].normal = normal;
						if (uvs.size() > 0)
							vertices[iVert].texcoord = uvs[face[iVert].uvID - 1];
						else
							vertices[iVert].texcoord = uv2f(0.f);
					}
					meshBVH->addTriangle(Triangle(vertices[0], vertices[1], vertices[2], material));
					meshBVH->addTriangle(Triangle(vertices[0], vertices[2], vertices[3], material));
					iVert += 4;
					totalTriCount += 2;
				}
				else
				{
					aka::Logger::error("Face type not supported, skipping : ", face.vertices.size());
				}
			}
		}
	}
	return true;
}

}
