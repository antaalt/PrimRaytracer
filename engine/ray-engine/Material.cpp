#include "Material.h"

#include "Physics.h"
#include "Sampler.h"

#include <cmath>


namespace app {
	namespace prim {
		
		Vector3 reflect(const Vector3 & incident, const Normal & normal)
		{
			return incident - 2.f * Vector3::dot(incident, normal) * normal;
		}
		Vector3 refract(const Vector3 & incident, const Normal & normal, float eta)
		{
			float R = 1.f - eta * eta * (1.f - Vector3::dot(incident, normal) * Vector3::dot(incident, normal));
			if (R < 0.f)
				return Vector3(0.f);
			else
				return eta * incident - (eta * Vector3::dot(incident, normal) + sqrtf(R)) * normal;
			// TODO use fresnel instead
			//float R = fresnel(incident, normal, eta); 
		}
		Vector3 sampleUnitSphere(const rand::Sampler &sampler)
		{
			float r1 = sampler.r1();
			float r2 = sampler.r2();
			
			float sinTheta = sqrtf(1 - r1 * r1);
			float phi = 2.f * M_PIf * r2;
			float x = sinTheta * cosf(phi);
			float z = sinTheta * sinf(phi);
			return Vector3(x, r1, z);
		}
		Vector3 sampleMicroFacet(const rand::Sampler &sampler, float roughness)
		{
			float r1 = sampler.r1();
			float r2 = sampler.r2();
			float phi = std::atanf(roughness * std::sqrtf(r1) / std::sqrtf(1 - r1));
			float theta = 2 * M_PIf * r2;
			Vector3 p;
			p.x = std::sinf(phi) * std::cosf(theta);
			p.y = std::sinf(phi) * std::sinf(theta);
			p.z = std::cosf(phi);
			return p;
		}
	}
}