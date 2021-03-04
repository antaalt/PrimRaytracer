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
	const vec4f camTarget = inverse * vec4f(screenPos.x, screenPos.y, 1, 1);

	const point3f camPos = point3f(0);
	const vec3f camDir = vec3f::normalize(vec3f(camTarget));

	if (lensRadius > 0)
	{
		vec2f pLens = lensRadius * sample::unitDisk(Rand::sample<float>(), Rand::sample<float>());
		float ft = focalDistance / camDir.z;
		point3f pFocus = camPos + camDir * ft;
		point3f origin = point3f(pLens.x, pLens.y, 0.f);
		return Ray(
			transform(origin),
			transform(vec3f::normalize(pFocus - origin))
		);
	}
	else
	{
		return Ray(
			transform(camPos),
			transform(camDir)
		);
	}
}

}
