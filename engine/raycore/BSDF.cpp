#include "BSDF.h"


namespace raycore {
	vec3 reflect(const vec3 & incident, const norm3 & normal)
	{
		return incident - 2.f * vec3::dot(incident, normal) * normal;
	}
	bool refract(vec3 &wt, const vec3 & wi, const norm3 & n, float eta)
	{
		float NdotV = vec3::dot(n, wi);
		float k = 1.f - eta * eta * (1.f - NdotV * NdotV);
		if (k < 0.f)
			return true;
		wt = vec3::normalize(eta * wi - (eta*NdotV + sqrtf(k)) * n);
		return false;
	}
	vec3 sampleUnitSphere(float r1, float r2)
	{
		const float phi = 2.f * M_PIf * r1;
		const float r = r2;
		return vec3(
			r * cosf(phi),
			sqrtf(1.f - r * r),
			r * sinf(phi)
		);
	}
	vec3 sampleMicroFacet(float roughness, float r1, float r2)
	{
		float phi = std::atanf(roughness * std::sqrtf(r1) / std::sqrtf(1 - r1));
		float theta = 2 * M_PIf * r2;
		vec3 p;
		p.x = std::sinf(phi) * std::cosf(theta);
		p.y = std::sinf(phi) * std::sinf(theta);
		p.z = std::cosf(phi);
		return p;
	}
}