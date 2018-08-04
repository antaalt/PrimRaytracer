#pragma once

#include <vector>
#include "Types.h"
#include "../engine/Image.h"
#include "Texture.h"

namespace app {

	enum class MaterialType {
		DIFFUSE,
		SPECULAR,
		DIELECTRIC,
		METAL,
		UNDEFINED_TYPE
	};

	struct Material {
		using Ptr = Material*;
		unsigned int index;
		Texture::Ptr texture;
		ColorHDR color;
		MaterialType type;
	};

	struct Vertex {
		Point3 position;
		Normal normal;
		Texcoord texcoord;
		ColorHDR color;

		Vertex() {}
		Vertex(Point3 p, Normal n, Texcoord t, ColorHDR c) : position(p), normal(n), texcoord(t), color(c) {}
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
		using Ptr = Primitive*;
		std::vector<Triangle> triangles;
		std::vector<Vertex> vertices;
		Material::Ptr material;
	};
	// --- Shapes
	enum class ShapeType {
		PARALLELOGRAM,
		SPHERE,
		MESH
	};
	struct Shape {
		using Ptr = Shape*;
		virtual ShapeType type() const = 0;
	};
	struct Parallelogram : public Shape {
		Point3 point[3];
		Normal normal;
		Material::Ptr material;
		virtual ShapeType type() const;
	};
	struct Sphere : public Shape {
		float radius;
		Point3 center;
		Material::Ptr material;
		virtual ShapeType type() const;
	};
	struct Mesh : public Shape {
		using Ptr = Mesh*;
		std::vector<Primitive::Ptr> primitives;
		virtual ShapeType type() const;
	};

	// --- Nodes
	struct Node {
		using Ptr = Node*;
		Shape::Ptr shape;
		Material::Ptr material;
		Matrix4 transform;
		Node::Ptr parent;
		std::vector<Node::Ptr> childrens;
		Node() : parent(nullptr) {}

		Matrix4 getModel() const;
	};

	struct Scene {
		std::vector<Primitive> primitives;
		std::vector<Shape::Ptr> shapes;
		//std::vector<Mesh> meshes;
		std::vector<Node> nodes;
		std::vector<Material> materials;
		std::vector<Texture> textures;

		Primitive &addPrimitive();
		Mesh &addMesh();
		Sphere &addSphere();
		Parallelogram &addParallelogram();
		Node &addNode();
		Material &addMaterial();
		Texture &addTexture(const std::vector<unsigned char> &data, unsigned int width, unsigned int height, unsigned int components);

		struct GLTF
		{
			static Scene load(std::string path);
			static bool write(const Scene &scene);
		};
	};
}


