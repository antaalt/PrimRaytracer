#pragma once
#include "Config.h"

namespace raycore {
	namespace sample {
		vec3 unitSphere(float r1, float r2);
		vec3 unitHemisphere(float r1, float r2);
		vec3 unitMicrofacet(float roughness, float r1, float r2);
		vec2 unitDisk(float r1, float r2);
	}
}

