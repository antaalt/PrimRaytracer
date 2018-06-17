#pragma once

#include "Node.h"

namespace Application {

class Camera : public Node
{
public:
	Camera();
	Camera(float fovy, float ratio);
	~Camera();

	void setProjection(float fovy, float ratio);
	void setProjectionOrtho();

	glm::mat4 getProjection() const;

	glm::mat4 getView() const;

	void setNearFar(glm::vec2 p_nearFar);

	glm::vec2 getNearFar();

	virtual void render(const glm::mat4 &p_model, const glm::mat4 &p_view, const glm::mat4 &p_projection);

private:
	glm::vec2 m_nearFar;
	glm::mat4 m_projection;
};

}
