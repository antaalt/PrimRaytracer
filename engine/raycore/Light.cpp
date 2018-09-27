#include "Light.h"

namespace raycore {
	namespace tracer {

		Light::Light(colorHDR albedo, float intensity) : albedo(albedo), intensity(intensity)
		{
		}

		Light::~Light()
		{
		}
	}
}
