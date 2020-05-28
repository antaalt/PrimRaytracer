#pragma once
#include "Config.h"

namespace raycore {
namespace sample {

vec3f unitSphere(float r1, float r2);
vec3f unitHemisphere(float r1, float r2);
vec3f unitMicrofacet(float roughness, float r1, float r2);
vec2f unitDisk(float r1, float r2);

}
}

