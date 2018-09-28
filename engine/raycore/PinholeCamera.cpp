#include "PinholeCamera.h"



namespace raycore {

	namespace tracer {
		
		PinholeCamera::PinholeCamera() : Camera()
		{
		}

		PinholeCamera::~PinholeCamera()
		{
		}
		Ray PinholeCamera::generateUnitRay(RayIndex x, RayIndex y) const
		{
			float xt = x.get();
			float yt = y.get();
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
