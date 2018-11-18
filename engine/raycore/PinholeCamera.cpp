#include "PinholeCamera.h"

#include "transform.h"

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
			float xt = x.get(); // [-1; 1]
			float yt = y.get();
			float dist = 1.f / math::tan(math::Degree(Config::fov * 0.5f));
			vec3 directionScreenSpace = normalize(vec3(
				u.x * xt + v.x * yt + dist * w.x,
				u.y * xt + v.y * yt + dist * w.y,
				u.z * xt + v.z * yt + dist * w.z
			));
			vec3 directionWorldSpace = transform::screenToWorld(directionScreenSpace);
			return Ray(transform::screenToWorld(eye), directionWorldSpace);
		}
		bool PinholeCamera::computeTransform()
		{
			if (!m_changed)
				return false;
			// screen space
			u = normalize(m_transform[0]);
			v = normalize(m_transform[1]);
			w = normalize(m_transform[2]);
			eye = point3(m_transform[3]);

			m_changed = false;
			return true;
		}
	}
}
