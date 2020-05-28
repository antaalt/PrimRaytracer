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
	float dist = 1.f / geometry::tan(geometry::radianf(geometry::degreef(Config::fov * 0.5f)));
	vec3f directionScreenSpace = vec3f::normalize(vec3f(
		u.x * xt + v.x * yt + dist * w.x,
		u.y * xt + v.y * yt + dist * w.y,
		u.z * xt + v.z * yt + dist * w.z
	));
	vec3f directionWorldSpace = transform::screenToWorld(directionScreenSpace);
	return Ray(transform::screenToWorld(eye), directionWorldSpace);
}
bool PinholeCamera::computeTransform()
{
	if (!m_changed)
		return false;
	// screen space
	u = norm3f::normalize(norm3f(m_transform[0]));
	v = norm3f::normalize(norm3f(m_transform[1]));
	w = norm3f::normalize(norm3f(m_transform[2]));
	eye = point3f(m_transform[3]);

	m_changed = false;
	return true;
}
}
}
