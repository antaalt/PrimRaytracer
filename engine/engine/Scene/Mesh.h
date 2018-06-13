#pragma once
#include "../Config.h"
#include "Node.h"
#include "Material.h"

#include <vector>
#include <string>


namespace World {

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
		unsigned int vertex[3];
		struct {
			unsigned int a, b, c;
		};
	};
#pragma pack(pop)

	enum Attributes {
		POSITION,
		NORMAL,
		/*TANGENT,
		BITANGENT,*/
		COLOR,
		TEXCOORD0,
		TEXCOORD1,
		TEXCOORD2,
		TEXCOORD3,
		NB_ATTRIBUTES
	};

	void* offset(Attributes att);
	

	class Mesh : public Node
	{
	public:
		Mesh();
		~Mesh();

		void setMaterial(Material *material);
		Material *getMaterial();

		void render(const glm::mat4 &p_model, const glm::mat4 &p_view, const glm::mat4 &p_projection);

	private:
		bool createVAO();
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
