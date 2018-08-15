#include "PinholeCamera.h"



namespace app {

	namespace tracer {
		
		PinholeCamera::PinholeCamera(unsigned int width, unsigned int height) : Camera(width, height)
		{
		}

		PinholeCamera::~PinholeCamera()
		{
		}
		Ray PinholeCamera::generateRay(unsigned int x, unsigned int y) const
		{
			float xt = x / static_cast<float>(m_width) * 2.f - 1.f;
			float yt = y / static_cast<float>(m_height) * 2.f - 1.f;
			vec3 direction = vec3::normalize(vec3(
				u.x * xt + v.x * yt + w.x,
				u.y * xt + v.y * yt + w.y,
				u.z * xt + v.z * yt + w.z
			));
			return Ray(eye, direction);
		}
		bool PinholeCamera::computeTransform()
		{
			if (!m_changed)
				return false;
			u = convert::toVec3(m_transform[0]);
			v = convert::toVec3(m_transform[1]);
			w = convert::toVec3(m_transform[2]);
			eye = convert::toVec3(m_transform[3]);
			m_changed = false;
			return true;
		}
	}
}
