#include "Mesh.h"

namespace World {

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
		//m_glProgram->use();
		/*if (m_material)
		{
			m_material->use();
			CShader * s = m_material->getShader();
			if (s && s->isValid())
			{
				// fill the matrices normal, model, modelView, ModelViewProjection, ... (all which contain model)
				glm::mat4 mv = p_view * p_model;
				glm::mat4 mvp = p_projection * mv;
				glm::mat3 normal = glm::inverseTranspose(glm::mat3(p_model));

				s->setMatrixUniform(enumMatrix::MODEL_MATRIX, p_model);
				s->setMatrixUniform(enumMatrix::MODELVIEW_MATRIX, mv);
				s->setMatrixUniform(enumMatrix::MODELVIEWPROJECTION_MATRIX, mvp);
				s->setMatrixUniform(enumMatrix::NORMAL_MATRIX, normal);
			}
		}*/
		/*if (m_vao)
		{
			// VAO has been created : bind VAO
			glBindVertexArray(m_vao);

			glDrawElements(GL_TRIANGLES, m_nbFaces * 3, GL_UNSIGNED_INT, 0);

			// Unbind all
			glBindVertexArray(0);
		}*/
		// Projection matrix
		glMatrixMode(GL_PROJECTION);			// COMPATIBILITY PROFILE ONLY
		glLoadIdentity();
		glLoadMatrixf(&p_projection[0][0]);		// COMPATIBILITY PROFILE ONLY

												// on passe en mode MODELVIEW : la matrice manipulée maintenant est une matrice de transformation pour les objets 
		glMatrixMode(GL_MODELVIEW);				// COMPATIBILITY PROFILE ONLY
		glm::mat4 mv = p_view * p_model;																// construct the modelView matrix
		glLoadMatrixf(&mv[0][0]);				// COMPATIBILITY PROFILE ONLY
												// Clear the screen and the depth buffer
		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClearDepth(1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(2.5);
		glBegin(GL_LINES);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(0.f, 0.f, 0.f);
			glVertex3f(1.f, 0.f, 0.f);
			
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0, 1.f, 0);
			
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.f, 0.f, 1.f);
		glEnd();

		glColor3f(0.f, 1.f, 0.f);
		glBegin(GL_TRIANGLES);
		glVertex3f(1.f, 1.f, 2.f);
		glVertex3f(-1.f, -1.f, 2.f);
		glVertex3f(-1.f, 1.f, 2.f);
		glEnd();
		//m_glProgram->doNotUse();
	}
	void * offset(Attributes att)
	{
		switch (att)
		{
		case Attributes::POSITION:
			return 0;
		case Attributes::NORMAL:
			return (void*)(sizeof(glm::vec3));
		case Attributes::COLOR:
			return (void*)(2 * sizeof(glm::vec3));
		case Attributes::TEXCOORD0:
			return (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec4));
		case Attributes::TEXCOORD1:
			return (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(glm::vec2));
		case Attributes::TEXCOORD2:
			return (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec4) + 2 * sizeof(glm::vec2));
		case Attributes::TEXCOORD3:
			return (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec4) + 3 * sizeof(glm::vec2));
		default:
			return 0;
		}
	}
}