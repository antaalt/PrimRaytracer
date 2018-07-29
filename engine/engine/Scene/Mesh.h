#pragma once
#include "../Config.h"
#include "../Enum.h"
#include "Material.h"

#include <vector>
#include <string>


namespace app {

#pragma pack(push,1)
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec4 color;
		glm::vec2 texCoord[4];

		Vertex() : 
			position(glm::vec3(0.f)), 
			normal(glm::vec3(0.f)), 
			color(glm::vec4(1.f))
		{
			for(int i = 0; i < 4; i++)
				texCoord[i] = glm::vec2(0.f);
		}
	};

	union Face {
		unsigned int data[3];
		struct {
			unsigned int A, B, C;
		};
		Face() {}
		Face(unsigned int a, unsigned int b, unsigned int c) : A(a), B(b), C(c) {}
	};
#pragma pack(pop)
	

	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		void setMaterial(Material *material);
		Material *getMaterial();

		void render(const glm::mat4 &p_model, const glm::mat4 &p_view, const glm::mat4 &p_projection);

		void addFace(Face face);
		void addVertex(Vertex vert);

	
		bool createVAO();
	private:
		void deleteVAO();
	private:
		std::vector<Face> m_faces;
		std::vector<Vertex> m_vertices;
		GLuint m_vboArrayBuffer;
		GLuint m_vboElementArrayBuffer;
		GLuint m_vao;

		bool m_has[Attributes::NB_ATTRIBUTES];
		Material *m_material;
	};
}
