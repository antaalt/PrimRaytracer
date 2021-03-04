#include "OrthographicCamera.h"


namespace prim {

Ray OrthographicCamera::generateRay(const RaySampler::Type & screenPos) const
{
	return Ray(
		transform(point3f(screenPos.x, screenPos.y, 0)), 
		transform(vec3f(0, 0, 1))
	);
}

}