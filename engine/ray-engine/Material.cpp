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
		bool refract(Vector3 &wt, const Vector3 & wi, const Normal & n, float eta)
		{
			float cosThetaI = Vector3::dot(wi, n);
			float sin2thetaI = std::max(0.f, 1.f - cosThetaI * cosThetaI);
			float sin2thetaT = eta * eta * sin2thetaI;
			if (sin2thetaT >= 1) return false;
			float cosThetaT = std::sqrt(1.f - sin2thetaT);
			wt = eta * -wi + (eta * cosThetaI - cosThetaT) * Vector3(n);
			return true;
			/*float IdotN = Vector3::dot(wi, normal);
			float k = 1.f - eta * eta * (1.f - IdotN * IdotN);
			if (k < 0.f)
				return true;
			wt = (eta * (wi + IdotN * normal) - normal * sqrt(k));*/
			return false;
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