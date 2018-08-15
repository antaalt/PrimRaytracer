#include "Light.h"

namespace app {
	namespace tracer {

		Light::Light()
		{
		}

		Light::~Light()
		{
		}
		bool Light::inShadow(const Ray & shadowRay, Accelerator * acc)
		{
			return acc->intersect(shadowRay);
		}
	}
}
