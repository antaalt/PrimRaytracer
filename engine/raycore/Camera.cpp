#include "Camera.h"
#include "transform.h"

namespace raycore {

namespace tracer {

Camera::Camera() : m_transform(mat4f::identity()), m_fov(DEFAULT_FOV)
{
}


Camera::~Camera()
{
}
void Camera::lookAt(const point3f & eye, const point3f & target, const vec3f &up)
{
	vec3f forward(transform::worldToScreen(target - eye));
	vec3f right(vec3f::cross(transform::worldToScreen(up), forward));
	vec3f upCoordinate(vec3f::cross(forward, right));
	// screen space
	m_transform = mat4f(
		col4f(vec3f::normalize(right), 0.f),
		col4f(vec3f::normalize(upCoordinate), 0.f),
		col4f(vec3f::normalize(forward), 0.f),
		col4f(transform::worldToScreen(eye), 1.f)
	);
	m_changed = true;
}
void Camera::rotate(const vec3f & axis, geometry::degreef angle)
{
	m_transform = m_transform * mat4f::rotate(axis,angle);
	m_changed = true;
}
void Camera::translate(const vec3f & translation)
{
	m_transform = m_transform * mat4f::translate(translation);
	m_changed = true;
}
void Camera::setFov(float fov)
{
	m_fov = fov;
}
}
}