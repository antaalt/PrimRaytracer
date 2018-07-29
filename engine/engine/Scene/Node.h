#pragma once


#include <vector>

#include "../Config.h"
#include "Mesh.h"

namespace app {

	class Node
	{
	public:
		Node();
		~Node();

		void translate(glm::vec3 translation);
		void rotate(float angle, glm::vec3 direction);
		void scale(glm::vec3 scale);

		void setLocalTransform(glm::mat4 &p_matrix);
		glm::mat4 getLocalTransform() const;

		void setTransform(glm::mat4 p_matrix, Node *p_ref = nullptr);
		glm::mat4 getTransform(Node *p_ref = nullptr) const;

		void setParent(Node *p_ref, bool p_keepTransform = false);
		void addChild(Node *p_node);
		void removeChild(Node *p_node);

		void setMesh(Mesh *mesh);
		Mesh *getMesh();

		Node * getParent() const;

		glm::mat4 getModel() const;

	public:
		void draw(const glm::mat4 &p_model, const glm::mat4 &p_view, const glm::mat4 &p_projection);

		virtual void render(const glm::mat4 &p_model, const glm::mat4 &p_view, const glm::mat4 &p_projection);

	protected:
		glm::mat4 m_localTransform;
		Node *m_parent;
		std::vector<Node*> m_children;
		Mesh *m_mesh;
	};

}