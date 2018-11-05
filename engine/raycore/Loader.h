#pragma once
#include <string>
#include "Scene.h"

namespace raycore {

	namespace loader {

		/*struct Shape {
			enum Type {
				MESH,
				SPHERE
			};
			struct bbox {
				point3 min, max;
			};
			virtual Type type() const = 0;
			int materialID;
		};
		struct Mesh : Shape {
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
			virtual Type type() const;
		};
		struct Sphere : Shape {
			point3 center;
			float radius;
			norm3 up;
			virtual Type type() const;
		};*/

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
		/*struct Node {
			mat4 transform;
			int shapeID;
			int parentID;
			std::vector<int> childrensID;
		};

		struct Scene {
			Scene();
			~Scene();
			std::vector<prim::Material*> materials; // pass it, no delete
			std::vector<Texture<colorHDR>*> textures; // pass it, no delete
			std::vector<Shape*> shapes; // no direct access
			std::vector<Node> nodes; // transform it then delete it
			Mesh &addMesh();
			Sphere &addSphere();
			Node &addNode();
			prim::Material *addMaterial(prim::Material *material);
			Texture<colorHDR> *addTexture(Texture<colorHDR> * texture);
		};*/
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
		//bool build(loader::Scene &scene);
		bool newBuild(prim::Scene &scene);
	}
}


