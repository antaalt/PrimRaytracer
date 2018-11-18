#pragma once
#include <string>
#include "Scene.h"

namespace raycore {

	namespace loader {
		struct Mesh {
			struct Face {
				union {
					unsigned int data[3];
					struct {
						unsigned int A, B, C;
					};
				};
				Face() {}
				Face(unsigned int A, unsigned int B, unsigned int C) : A(A), B(B), C(C) {}
			};

			struct Vertex {
				point3 position;
				norm3 normal;
				uv2 uv;
				colorHDR color;
			};
			std::vector<Vertex> vertices;
			std::vector<Face> faces;
		};

		struct Sphere {
			point3 center;
			float radius;
			norm3 up;
		};

		struct Primitive {
			Primitive();
			// Mesh constructor
			Primitive(const Mesh &mesh, prim::Material *material);
			// Sphere constructor
			Primitive(const Sphere &sphere, prim::Material *material);
			~Primitive();

			std::vector<prim::Hitable*> hitables;
		};
	}

	namespace GLTF {
		bool load(std::string path, prim::Scene &scene);
		bool write(std::string path, const prim::Scene &scene);
	}

	namespace OBJ {
		struct FaceID {
			struct ID { unsigned int data[3]; };
			ID posID;
			ID normID;
			ID uvID;
		};
		bool load(std::string path, loader::Mesh &mesh);
		bool write(std::string path, const loader::Mesh &mesh);
	}

	namespace builder {
		bool build(prim::Scene &scene);
	}
}


