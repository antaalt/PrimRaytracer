#include "PerspectiveCamera.h"

namespace prim {

Ray PerspectiveCamera::generateRay(const RaySampler::Type & screenPos) const
{
	const vec4f camPos = transform * vec4f(0, 0, 0, 1);
	const vec4f camTarget = perspective * vec4f(screenPos.x, screenPos.y, 1, 1);
	const vec4f camDir = transform * vec4f(vec3f::normalize(vec3f(camTarget)), 0);
	Ray ray;
	ray.origin = point3f(camPos);
	ray.direction = vec3f(camDir);
	return ray;
}

}
