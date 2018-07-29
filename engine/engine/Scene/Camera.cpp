#include "Camera.h"


namespace app {

	Camera::Camera() : m_nearFar(glm::vec2(0.01f, 100.f))
	{
		setProjectionOrtho();
	}

	Camera::Camera(float fovy, float ratio)
	{
		setProjection(fovy, ratio);
	}

	Camera::~Camera()
	{
	}

	void Camera::setProjection(float fovy, float ratio)
	{
		m_projection = glm::perspective(fovy, ratio, m_nearFar.x, m_nearFar.y);
	}

	void Camera::setProjectionOrtho()
	{
		m_projection = glm::ortho(-1.f, 1.f, -1.f, 1.f);
	}


	glm::mat4 Camera::getProjection() const
	{
		return m_projection;
	}

	glm::mat4 Camera::getView() const
	{
		return glm::inverse(getModel());
	}

	void Camera::setNearFar(glm::vec2 p_nearFar)
	{
		m_nearFar = p_nearFar;
	}

	glm::vec2 Camera::getNearFar()
	{
		return m_nearFar;
	}

	void Camera::render(const glm::mat4 & p_model, const glm::mat4 & p_view, const glm::mat4 & p_projection)
	{
	}

}