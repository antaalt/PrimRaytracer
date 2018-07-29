#pragma once
#include "Hitable.h"
#include "Material.h"

#include <vector>

namespace app {

	namespace prim {

		/*struct Vertex {
			Point3 position;
			Normal normal;
			Texcoord texcoord;
		};

		class Triangle {
		public:
			union {
				unsigned int vertices[3];
				struct {
					unsigned int A, B, C;
				};
			};

			Triangle();
			Triangle(unsigned int v1, unsigned int v2, unsigned int v3);
		};

		class Primitive : public Hitable
		{
		public:
			using Ptr = Primitive*;
			Primitive();
			~Primitive();

			virtual bool intersect(const tracer::Ray &ray, Intersection &intersection) const;

			std::vector<Vertex> vertices;
			std::vector<Triangle> triangles;
			Material::Ptr material;
		};*/
	}
}