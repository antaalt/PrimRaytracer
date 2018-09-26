#pragma once

#include <vector>
#include "Types.h"
#include "Texture.h"

namespace raycore {

	enum class MaterialType {
		DIFFUSE,
		SPECULAR,
		DIELECTRIC,
		METAL,
		UNDEFINED_TYPE
	};

	struct Material {
		unsigned int index;
		Texture32* texture;
		colorHDR color;
		MaterialType type;
	};

	struct Vertex {
		point3 position;
		norm3 normal;
		uv2 texcoord;
		colorHDR color;

		Vertex() {}
		Vertex(point3 p, norm3 n, uv2 t, colorHDR c) : position(p), normal(n), texcoord(t), color(c) {}
	};

	struct Triangle {
		union {
			unsigned int vertices[3];
			struct {
				unsigned int A, B, C;
			};
		};

		Triangle();
		Triangle(unsigned int v1, unsigned int v2, unsigned int v3);
	};
	// --- Primitive
	struct Primitive {
		std::vector<Triangle> triangles;
		std::vector<Vertex> vertices;
		Material* material;
	};
	// --- Shapes
	enum class ShapeType {
		PARALLELOGRAM,
		SPHERE,
		MESH
	};
	struct Shape {
		virtual ShapeType type() const = 0;
	};
	struct Parallelogram : public Shape {
		point3 point[3];
		norm3 normal;
		Material* material;
		virtual ShapeType type() const;
	};
	struct Sphere : public Shape {
		float radius;
		point3 center;
		Material* material;
		virtual ShapeType type() const;
	};
	struct Mesh : public Shape {
		std::vector<Primitive*> primitives;
		virtual ShapeType type() const;
	};

	// --- Nodes
	struct Node {
		Shape* shape;
		Material* material;
		mat4 transform;
		Node* parent;
		std::vector<Node*> childrens;
		Node() : parent(nullptr), material(nullptr), shape(nullptr) {}

		mat4 getModel() const;
	};

	struct Light {
		vec3 position;
	};

	struct Scene {
		Scene();
		~Scene();
		Scene(const Scene& other) = delete;
		Scene& operator=(const Scene &other) = delete;

		std::vector<Primitive> primitives;
		std::vector<Shape*> shapes;
		std::vector<Node> nodes;
		std::vector<Material> materials;
		std::vector<Texture32> textures;
		std::vector<Light> lights;

		Primitive &addPrimitive();
		Mesh &addMesh();
		Sphere &addSphere();
		Parallelogram &addParallelogram();
		Node &addNode();
		Material &addMaterial();
		Texture32 &addTexture(const std::vector<unsigned char> &data, unsigned int width, unsigned int height, unsigned int components);
		Light &addLight();

		struct GLTF
		{
			static bool load(std::string path, Scene &scene);
			static bool write(const Scene &scene);
		};
	};
}


