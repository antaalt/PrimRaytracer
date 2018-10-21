#pragma once

#include <vector>
#include "Config.h"
#include "Texture.h"
#include "TextureMap.h"
#include "ConstantTexture.h"
#include "Checker.h"
#include "Matte.h"
#include "Metal.h"
#include "Glass.h"

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
		Texture<colorHDR>* texture;
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
		norm3 up;
		Material* material;
		virtual ShapeType type() const;
	};
	struct Mesh : public Shape {
		std::vector<Primitive> primitives;
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
		point3 position;
	};

	struct Scene {
		Scene();
		~Scene();
		Scene(const Scene& other) = delete;
		Scene& operator=(const Scene &other) = delete;

		std::vector<Shape*> shapes;
		std::vector<Node> nodes;
		std::vector<Material> materials;
		std::vector<Texture<colorHDR>*> textures;
		std::vector<Light> lights;

		Mesh &addMesh();
		Sphere &addSphere();
		Parallelogram &addParallelogram();
		Node &addNode();
		Material &addMaterial();
		Texture<colorHDR> *addTexture(Texture<colorHDR> * texture);
		Light &addLight();
	};
}


