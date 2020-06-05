#include "OBJLoader.h"

#include <sstream>
#include <array>
#include <map>
#include <fstream>

#include "MeshBVH.h"

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

struct Material {

};

struct Group {
	std::string name;
	std::vector<Face> faces;
	Material *material;
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

bool OBJLoader::load(Reader & reader, Scene &scene)
{
	std::vector<point3f> positions;
	std::vector<norm3f> normals;
	std::vector<uv2f> uvs;
	std::vector<obj::Object> objects;
	std::map<std::string, obj::Material> materials;

	std::stringstream file;
	file << reader.data();
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
			// TODO load materials
			continue;
		}
		else if (header == "usemtl")
		{
			std::string materialName;
			ss >> materialName;
			// TODO ensure material exist.
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
			Log::warn("Unknown data : ", line);
			break;
		}
	}
	// TODO load mtl
	size_t totalTriCount = 0;
	for (const obj::Object &object : objects)
	{
		//Texture<float> *texture = new ConstantTexture<float>(color4f(0.5f, 0.5f, 0.5f, 1.f));
		Texture4f *texture = new ImageTexture4f("../prim/data/textures/1.jpg");
		//Texture4f *texture = new ImageTexture4f("../prim/data/models/aya/tex/091_W_Aya_2K_01.jpg");
		Material *material = new Matte(texture);
		MeshBVH *meshBVH = new MeshBVH(object.name, geometry::mat4f::identity(), material);
		scene.textures.push_back(texture);
		scene.materials.push_back(material);
		scene.hitables.push_back(meshBVH);
		uint32_t iVert = 0;
		for (const obj::Group &group : object.groups)
		{
			for (const obj::Face &face : group.faces)
			{
				if (face.vertices.size() == 3)
				{
					// It's a triangle !
					vec3f AB(positions[face[1].posID - 1] - positions[face[0].posID - 1]);
					vec3f AC(positions[face[2].posID - 1] - positions[face[0].posID - 1]);
					norm3f normal(vec3f::normalize(vec3f::cross(AB, AC)));
					for (size_t iVert = 0; iVert < 3; iVert++)
					{
						meshBVH->addPosition(positions[face[iVert].posID - 1]);
						if (normals.size() > 0)
							meshBVH->addNormal(normals[face[iVert].normID - 1]);
						else
							meshBVH->addNormal(normal);
						if (uvs.size() > 0)
							meshBVH->addUV(uvs[face[iVert].uvID - 1]);
						else
							meshBVH->addUV(uv2f(0.f));
						meshBVH->addColor(color4f(1.f));
					}
					meshBVH->addTriangle(MeshBVH::Triangle{
						static_cast<uint32_t>(iVert) + 0,
						static_cast<uint32_t>(iVert) + 1,
						static_cast<uint32_t>(iVert) + 2
					});
					iVert += 3;
					totalTriCount += 1;
				}
				else if (face.vertices.size() == 4)
				{
					// It's a quad !
					vec3f AB(positions[face[1].posID - 1] - positions[face[0].posID - 1]);
					vec3f AC(positions[face[2].posID - 1] - positions[face[0].posID - 1]);
					norm3f normal(vec3f::normalize(vec3f::cross(AB, AC)));
					for (size_t iVert = 0; iVert < 4; iVert++)
					{
						meshBVH->addPosition(positions[face[iVert].posID - 1]);
						if (normals.size() > 0)
							meshBVH->addNormal(normals[face[iVert].normID - 1]);
						else
							meshBVH->addNormal(normal);
						if (uvs.size() > 0)
							meshBVH->addUV(uvs[face[iVert].uvID - 1]);
						else
							meshBVH->addUV(uv2f(0.f));
						meshBVH->addColor(color4f(1.f));
					}
					meshBVH->addTriangle(MeshBVH::Triangle{
						static_cast<uint32_t>(iVert) + 0,
						static_cast<uint32_t>(iVert) + 1,
						static_cast<uint32_t>(iVert) + 2
					});
					meshBVH->addTriangle(MeshBVH::Triangle{
						static_cast<uint32_t>(iVert) + 0,
						static_cast<uint32_t>(iVert) + 2,
						static_cast<uint32_t>(iVert) + 3
					});
					iVert += 4;
					totalTriCount += 2;
				}
				else
				{
					Log::error("Face type not supported, skipping : ", face.vertices.size());
				}
			}
		}
	}
	return true;
}

}
