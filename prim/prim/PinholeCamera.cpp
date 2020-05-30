#include "PinholeCamera.h"

#include "Transform.h"

namespace prim {

Ray PinholeCamera::generateRay(const RaySampler::Type & screenPos) const
{
	float dist = 1.f / geometry::tan(geometry::radianf(geometry::degreef(hFov * 0.5f)));
	vec3f directionScreenSpace = vec3f::normalize(vec3f(
		u.x * screenPos.x + v.x * screenPos.y + dist * w.x,
		u.y * screenPos.x + v.y * screenPos.y + dist * w.y,
		u.z * screenPos.x + v.z * screenPos.y + dist * w.z
	));
	vec3f directionWorldSpace = transform::screenToWorld(directionScreenSpace);
	return Ray(transform::screenToWorld(eye), directionWorldSpace);
}

}
