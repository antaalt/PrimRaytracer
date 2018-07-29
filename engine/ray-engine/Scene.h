#pragma once

#include <vector>
#include "Types.h"
#include "../engine/Image.h"

namespace app {

	struct Texture {
		using Ptr = Texture*;
		Texture();
		Texture(const Image &image);
		ColorHDR texture2D(float u, float v);
		unsigned int stride();
	private:
		std::vector<float> m_data;
		unsigned int m_width, m_height;
		unsigned int m_component;
	};

	enum class MaterialType {
		DIFFUSE,
		SPECULAR,
		DIELECTRIC,
		METAL,
		UNDEFINED_TYPE
	};

	struct Material {
		unsigned int index;
		using Ptr = Material*;
		Texture::Ptr texture;
		ColorHDR color;
		MaterialType type;
	};

	struct Vertex {
		Point3 position;
		Normal normal;
		Texcoord texcoord;
		ColorHDR color;
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
		PLANE,
		SPHERE,
		MESH
	};
	struct Shape {
		using Ptr = Shape*;
		virtual ShapeType type() const = 0;
	};
	struct Plane : public Shape {
		Point3 point;
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
		Node &addNode();
		Material &addMaterial();
		Texture &addTexture();

		struct GLTF
		{
			static Scene load(std::string path);
			static bool write(const Scene &scene);
		};
	};
}


