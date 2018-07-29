#include "Mesh.h"

namespace app {

	Mesh::Mesh()
	{
	}


	Mesh::~Mesh()
	{
	}
	
	bool Mesh::createVAO()
	{
		if (!m_vertices.size() || !m_faces.size())
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
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &m_vboElementArrayBuffer);
		if (!m_vboElementArrayBuffer)
		{
			deleteVAO();
			return false;
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboElementArrayBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_faces.size() * 3 * sizeof(unsigned int), &m_faces[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(POSITION);
		glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offset(POSITION));
		if (m_has[Attributes::NORMAL])
		{
			glEnableVertexAttribArray(NORMAL);
			glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), offset(NORMAL));
		}
		if (m_has[Attributes::COLOR])
		{
			glEnableVertexAttribArray(COLOR);
			glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), offset(COLOR));
		}

		for (GLuint att = Attributes::TEXCOORD0; att < NB_ATTRIBUTES; att++)
		{
			if (m_has[att])
			{
				glEnableVertexAttribArray(att);
				glVertexAttribPointer(att, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offset(static_cast<Attributes>(att)));
			}
		}

		glBindVertexArray(0);

		return true;
	}
	void Mesh::deleteVAO()
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
	void Mesh::setMaterial(Material * material)
	{
		m_material = material;
	}
	Material * Mesh::getMaterial()
	{
		return m_material;
	}
	void Mesh::render(const glm::mat4 & p_model, const glm::mat4 & p_view, const glm::mat4 & p_projection)
	{
		if (m_material != nullptr)
		{
			m_material->use();
			GL::Program * p = m_material->getProgram();
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

			glDrawElements(GL_TRIANGLES, m_faces.size() * 3, GL_UNSIGNED_INT, 0);

			// Unbind all
			glBindVertexArray(0);
		}
	}
	void Mesh::addFace(Face face)
	{
		m_faces.push_back(face);
	}
	void Mesh::addVertex(Vertex vert)
	{
		m_vertices.push_back(vert);
	}
	
}