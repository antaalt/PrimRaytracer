#include "Node.h"

#include <algorithm>


namespace app {

	Node::Node() : m_localTransform(glm::mat4(1.f)), m_parent(nullptr)
	{
	}


	Node::~Node()
	{

	}

	void Node::translate(glm::vec3 translation)
	{
		m_localTransform = glm::translate(m_localTransform, translation);
	}

	void Node::rotate(float angle, glm::vec3 direction)
	{
		m_localTransform = glm::rotate(m_localTransform, angle, direction);
	}

	void Node::scale(glm::vec3 scale)
	{
		m_localTransform = glm::scale(m_localTransform, scale);
	}

	void Node::setLocalTransform(glm::mat4 &p_matrix)
	{
		m_localTransform = p_matrix;
	}

	glm::mat4 Node::getLocalTransform() const
	{
		return m_localTransform;
	}

	void Node::setTransform(glm::mat4 p_matrix, Node * p_ref)
	{
		if (m_parent == nullptr)
		{
			if (p_ref == nullptr)
				setLocalTransform(p_matrix);
			else
				setLocalTransform(p_ref->getModel() * p_matrix);
		}
		else
		{
			if (p_ref == nullptr)
				setLocalTransform(glm::inverse(m_parent->getModel()) * p_matrix);
			else
				setLocalTransform(glm::inverse(m_parent->getModel()) *  p_ref->getModel() * p_matrix);
		}
	}

	glm::mat4 Node::getTransform(Node * p_ref) const
	{
		if (p_ref == nullptr)
			return getModel();
		else
			return glm::inverse(p_ref->getModel()) * getModel();
	}

	void Node::setParent(Node * p_ref, bool p_keepTransform)
	{
		if (m_parent == p_ref)
			return;
		glm::mat4 m;
		if (!p_keepTransform)
			m = getModel();

		if (m_parent)
			m_parent->removeChild(this);

		m_parent = p_ref;
		if (!p_keepTransform)
			setTransform(m);

		if (m_parent)
			m_parent->addChild(this);
	}

	void Node::addChild(Node * p_node)
	{
		std::vector<Node*>::iterator position = std::find(m_children.begin(), m_children.end(), p_node);
		if (position != m_children.end())
			return;
		m_children.push_back(p_node);
	}

	void Node::removeChild(Node * p_node)
	{
		std::vector<Node*>::iterator position = std::find(m_children.begin(), m_children.end(), p_node);
		if (position != m_children.end())
			m_children.erase(position);
	}

	void Node::setMesh(Mesh * mesh)
	{
		m_mesh = mesh;
	}

	Mesh * Node::getMesh()
	{
		return m_mesh;
	}

	Node * Node::getParent() const
	{
		return m_parent;
	}

	glm::mat4 Node::getModel() const
	{
		if (m_parent == nullptr)
			return m_localTransform;
		else
			return m_parent->getModel() * m_localTransform;
	}

	void Node::draw(const glm::mat4 &p_model, const glm::mat4 &p_view, const glm::mat4 &p_projection)
	{
		const glm::mat4 model = p_model * m_localTransform;
		render(model, p_view, p_projection);
		for (size_t iChild = 0; iChild < m_children.size(); iChild++)
		{
			Node * child = m_children[iChild];
			child->draw(model, p_view, p_projection);
		}
	}

	void Node::render(const glm::mat4 & p_model, const glm::mat4 & p_view, const glm::mat4 & p_projection)
	{
		if (m_mesh != nullptr)
		{
			const glm::mat4 model = p_model * m_localTransform;
			m_mesh->render(model, p_view, p_projection);
		}
		else
		{
			// TODO render referential
		}
	}

}