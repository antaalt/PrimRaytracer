#include "BSDF.h"


namespace raycore {
	vec3 reflect(const vec3 & incident, const norm3 & normal)
	{
		return incident - 2.f * dot(incident, normal) * normal;
	}
	bool refract(vec3 &wt, const vec3 & wi, const norm3 & n, float eta)
	{
		float NdotV = dot(wi, n);
		float k = 1.f - eta * eta * (1.f - NdotV * NdotV);
		if (k < 0.f)
			return true;
		wt = normalize(eta * wi - (eta*NdotV + sqrtf(k)) * n);
		return false;
	}
}