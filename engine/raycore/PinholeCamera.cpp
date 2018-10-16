#include "PinholeCamera.h"



namespace raycore {

	namespace tracer {
		
		PinholeCamera::PinholeCamera(float aperture, float focalDist) : Camera(), aperture(aperture), focalDist(focalDist)
		{
		}

		PinholeCamera::~PinholeCamera()
		{
		}
		Ray PinholeCamera::generateRay(RayIndex x, RayIndex y) const
		{
			float xt = x.get();
			float yt = y.get();
			float dist = 1.f / tanf(Config::fov * 0.5f * M_PIf / 180.f);
			vec3 direction = vec3::normalize(vec3(
				u.x * xt + v.x * yt + dist * w.x,
				u.y * xt + v.y * yt + dist * w.y,
				u.z * xt + v.z * yt + dist * w.z
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
