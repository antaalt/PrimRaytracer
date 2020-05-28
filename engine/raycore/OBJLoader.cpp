#include "OBJLoader.h"

#include <sstream>
#include <array>

#include "Triangle.h"

namespace raycore {

struct FaceID {
	struct ID { unsigned int data[3]; };
	ID posID;
	ID normID;
	ID uvID;
};

bool OBJLoader::load(Reader & reader, prim::Scene &scene)
{
	std::vector<point3> positions;
	std::vector<norm3> normals;
	std::vector<uv2> uvs;
	std::vector<FaceID> faces;

	auto coordinates = [](const point3 &p) -> point3
	{
		return point3(p.x, -p.z, p.y);
	};
	auto coordinatesN = [](const norm3 &p) -> norm3
	{
		return norm3(p.x, -p.z, p.y);
	};
	std::stringstream file;
	file << reader.data();
	std::string line;
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
	// TODO load mtl
	Texture<colorHDR> *texture = new ConstantTexture<colorHDR>(colorHDR(0.5f, 0.5f, 0.5f, 1.f));
	prim::Material *material = new prim::Matte(texture);
	scene.addTexture(texture);
	scene.addMaterial(material);
	std::vector<prim::Hitable*> hitables;
	for (size_t iTri = 0; iTri < faces.size(); iTri++)
	{
		FaceID &face = faces[iTri];
		vec3 AB(positions[face.posID.data[1] - 1] - positions[face.posID.data[0] - 1]);
		vec3 AC(positions[face.posID.data[2] - 1] - positions[face.posID.data[0] - 1]);
		norm3 normal(normalize(cross(AB, AC)));
		std::array<prim::Vertex, 3> vertices;
		for (size_t iVert = 0; iVert < 3; iVert++)
		{
			prim::Vertex vert;
			vert.position = coordinates(positions[face.posID.data[iVert] - 1]);
			if (normals.size() > 0)
				vert.normal = coordinatesN(normals[face.normID.data[iVert] - 1]);
			else
				vert.normal = coordinatesN(normal);
			if (uvs.size() > 0)
				vert.texcoord = uvs[face.uvID.data[iVert] - 1];
			else
				vert.texcoord = uv2(0.f);
			vert.color = colorHDR(1.f);
			vertices[iVert] = vert;
		}
		prim::Triangle *tri = new prim::Triangle(
			vertices[0],
			vertices[1],
			vertices[2]
		);
		tri->material = material; 
		hitables.push_back(tri);
	}
	prim::Group * root = scene.getRoot();
	ASSERT(root != nullptr, "Root is null");
	prim::Geometry *geometry = new prim::Geometry(hitables);
	geometry->setAcceleration(prim::Acceleration::BVH);
	root->addChild(geometry);
	return true;
}

}
