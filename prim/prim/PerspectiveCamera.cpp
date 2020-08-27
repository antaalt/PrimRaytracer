#include "PerspectiveCamera.h"

namespace prim {

PerspectiveCamera::PerspectiveCamera() :
	perspective(mat4f::perspective(degreef(60.f), 16.f / 9.f, 0.1f, 1000.f)),
	hFov(60.f),
	lensRadius(0.01f),
	focalDistance(10.f)
{
}

Ray PerspectiveCamera::generateRay(const RaySampler::Type & screenPos) const
{
	const vec4f camPos = vec4f(0, 0, 0, 1);
	const vec4f camTarget = perspective * vec4f(screenPos.x, screenPos.y, 1, 1);
	const vec4f camDir = vec4f(vec3f::normalize(vec3f(camTarget)), 0);

	if (lensRadius > 0)
	{
		vec2f pLens = lensRadius * sample::unitDisk(Rand::sample<float>(), Rand::sample<float>());
		float ft = focalDistance / camDir.z;
		point3f pFocus = point3f(camPos) + point3f(camDir) * ft;
		point3f origin = point3f(pLens.x, pLens.y, 0.f);
		return Ray(
			transform(origin),
			transform(vec3f::normalize(vec3f(pFocus - origin)))
		);
	}
	else
	{
		return Ray(
			transform(point3f(camPos)),
			transform(vec3f(camDir))
		);
	}
}

}
