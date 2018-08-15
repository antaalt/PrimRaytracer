#include "Mesh.h"

namespace app {

	Mesh::Mesh()
	{
	}

	Mesh::~Mesh()
	{
	}
	Primitive & Mesh::addPrimitive()
	{
		primitives.emplace_back();
		return primitives.back();
	}
	void Mesh::render(const glm::mat4 & p_model, const glm::mat4 & p_view, const glm::mat4 & p_projection)
	{
		for (size_t iPrim = 0; iPrim < primitives.size(); iPrim++)
			primitives[iPrim].render(p_model, p_view, p_projection);
	}

	Primitive::Primitive()
	{

	}
	Primitive::~Primitive()
	{
		deleteVAO();
	}
	bool Primitive::createVAO()
	{
		if (!vertices.size() || !triangles.size())
			return false;

		if (!glewIsExtensionSupported("GL_ARB_vertex_array_object GL_ARB_vertex_buffer_object"))
			return true;

		glGenVertexArrays(1, &m_vao);
		if (!m_vao)
			return false;
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vboArrayBuffer);
		if (!m_vboArrayBuffer)
		{
			deleteVAO();
			return false;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_vboArrayBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &m_vboElementArrayBuffer);
		if (!m_vboElementArrayBuffer)
		{
			deleteVAO();
			return false;
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboElementArrayBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * 3 * sizeof(unsigned int), &triangles[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(POSITION);
		glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offset(POSITION));
		//if (m_has[Attributes::NORMAL])
		//{
			glEnableVertexAttribArray(NORMAL);
			glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), offset(NORMAL));
		//}
		//if (m_has[Attributes::COLOR])
		//{
			glEnableVertexAttribArray(COLOR);
			glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), offset(COLOR));
		//}

		for (GLuint att = Attributes::TEXCOORD0; att < NB_ATTRIBUTES; att++)
		{
			//if (m_has[att])
			//{
				glEnableVertexAttribArray(att);
				glVertexAttribPointer(att, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offset(static_cast<Attributes>(att)));
			//}
		}

		glBindVertexArray(0);

		return true;
	}

	void Primitive::deleteVAO()
	{
		if (m_vao)
			glDeleteVertexArrays(1, &m_vao);
		if (m_vboArrayBuffer)
			glDeleteBuffers(1, &m_vboArrayBuffer);
		if (m_vboElementArrayBuffer)
			glDeleteBuffers(1, &m_vboElementArrayBuffer);

		m_vao = 0;
		m_vboArrayBuffer = m_vboElementArrayBuffer = 0;
	}

	void Primitive::render(const glm::mat4 & p_model, const glm::mat4 & p_view, const glm::mat4 & p_projection)
	{
		if (material != nullptr)
		{
			material->use();
			GL::Program * p = material->getProgram();
			if (p != nullptr && p->isValid())
			{
				glm::mat4 mv = p_view * p_model;
				glm::mat4 mvp = p_projection * mv;
				//glm::mat3 normal = glm::inverseTranspose(glm::mat3(p_model));

				p->updateMatrixUniform(GL::MatrixUniform::MATRIX_MODEL, p_model);
				p->updateMatrixUniform(GL::MatrixUniform::MATRIX_VIEW, p_view);
				p->updateMatrixUniform(GL::MatrixUniform::MATRIX_PROJECTION, p_projection);
				p->updateMatrixUniform(GL::MatrixUniform::MATRIX_MVP, mvp);
			}
			else
				Log::warn("No valid program set");
		}
		if (m_vao)
		{
			// VAO has been created : bind VAO
			glBindVertexArray(m_vao);

			glDrawElements(GL_TRIANGLES, triangles.size() * 3, GL_UNSIGNED_INT, 0);

			// Unbind all
			glBindVertexArray(0);
		}
	}

}