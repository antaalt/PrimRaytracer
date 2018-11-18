#include "Camera.h"
#include "transform.h"

namespace raycore {

	namespace tracer {

		Camera::Camera() : m_transform(mat4::identity()), m_fov(DEFAULT_FOV)
		{
		}


		Camera::~Camera()
		{
		}
		void Camera::lookAt(const point3 & eye, const point3 & target, const vec3 &up)
		{
			vec3 forward(transform::worldToScreen(target - eye));
			vec3 right(cross(transform::worldToScreen(up), forward));
			vec3 upCoordinate(cross(forward, right));
			// screen space
			m_transform = mat4(
				mat4::col(normalize(right), 0.f),
				mat4::col(normalize(upCoordinate), 0.f),
				mat4::col(normalize(forward), 0.f),
				mat4::col(transform::worldToScreen(eye), 1.f)
			);
			m_changed = true;
		}
		void Camera::rotate(const vec3 & axis, math::Degree angle)
		{
			m_transform = m_transform * mat4::rotate(axis,angle);
			m_changed = true;
		}
		void Camera::translate(const vec3 & translation)
		{
			m_transform = m_transform * mat4::translate(translation);
			m_changed = true;
		}
		void Camera::setFov(float fov)
		{
			m_fov = fov;
		}
	}
}