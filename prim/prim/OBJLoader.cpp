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
		Texture4f *texture = new ConstantTexture4f(color4f(0.5f, 0.5f, 0.5f, 1.f));
		//Texture4f *texture = new ImageTexture4f("../prim/data/textures/1.jpg");
		//Texture4f *texture = new ImageTexture4f("../prim/data/models/aya/tex/091_W_Aya_2K_01.jpg");
		Material *material = new Matte(texture);
		MeshBVH *meshBVH = new MeshBVH(material);
		TransformNode *node = new TransformNode(mat4f::identity(), meshBVH);
		scene.textures.push_back(texture);
		scene.materials.push_back(material);
		scene.nodes.push_back(node);
		uint32_t iVert = 0;
		for (const obj::Group &group : object.groups)
		{
			for (const obj::Face &face : group.faces)
			{
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
					Log::error("Face type not supported, skipping : ", face.vertices.size());
				}
			}
		}
	}
	return true;
}

}
