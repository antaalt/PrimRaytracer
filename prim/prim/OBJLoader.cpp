#include "OBJLoader.h"

#include <sstream>
#include <array>

#include "MeshBVH.h"

namespace prim {

struct FaceID {
	struct ID { unsigned int data[3]; };
	ID posID;
	ID normID;
	ID uvID;
};

bool OBJLoader::load(Reader & reader, Scene &scene)
{
	std::vector<point3f> positions;
	std::vector<norm3f> normals;
	std::vector<uv2f> uvs;
	std::vector<FaceID> faces;

	auto coordinates = [](const point3f &p) -> point3f
	{
		return point3f(p.x, -p.z, p.y);
	};
	auto coordinatesN = [](const norm3f &p) -> norm3f
	{
		return norm3f(p.x, -p.z, p.y);
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
	Texture<float> *texture = new ConstantTexture<float>(color4f(0.5f, 0.5f, 0.5f, 1.f));
	Material *material = new Matte(texture);
	MeshBVH *meshBVH = new MeshBVH(geometry::mat4f::identity(), material);
	scene.textures.push_back(texture);
	scene.materials.push_back(material);
	scene.hitables.push_back(meshBVH);
	for (size_t iTri = 0; iTri < faces.size(); iTri++)
	{
		FaceID &face = faces[iTri];
		vec3f AB(positions[face.posID.data[1] - 1] - positions[face.posID.data[0] - 1]);
		vec3f AC(positions[face.posID.data[2] - 1] - positions[face.posID.data[0] - 1]);
		norm3f normal(vec3f::normalize(vec3f::cross(AB, AC)));
		for (size_t iVert = 0; iVert < 3; iVert++)
		{
			meshBVH->addPosition(coordinates(positions[face.posID.data[iVert] - 1]));
			if (normals.size() > 0)
				meshBVH->addNormal(coordinatesN(normals[face.normID.data[iVert] - 1]));
			else
				meshBVH->addNormal(coordinatesN(normal));
			if (uvs.size() > 0)
				meshBVH->addUV(uvs[face.uvID.data[iVert] - 1]);
			else
				meshBVH->addUV(uv2f(0.f));
			meshBVH->addColor(color4f(1.f));
		}
		meshBVH->addTriangle(MeshBVH::Triangle{
			static_cast<uint32_t>(iTri) * 3 + 0,
			static_cast<uint32_t>(iTri) * 3 + 1,
			static_cast<uint32_t>(iTri) * 3 + 2
		});
	}
	return true;
}

}
