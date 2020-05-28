#include "BSDF.h"


namespace raycore {

vec3f reflect(const vec3f & incident, const norm3f & normal)
{
	vec3f n(normal);
	return incident - 2.f * vec3f::dot(incident, n) * n;
}
bool refract(vec3f &wt, const vec3f & wi, const norm3f & n, float eta)
{
	float NdotV = vec3f::dot(wi, vec3f(n));
	float k = 1.f - eta * eta * (1.f - NdotV * NdotV);
	if (k < 0.f)
		return true;
	wt = vec3f::normalize(eta * wi - (eta*NdotV + geometry::sqrt(k)) * vec3f(n));
	return false;
}

}