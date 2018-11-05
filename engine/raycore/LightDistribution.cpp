#include "LightDistribution.h"
#include "Scene.h"

namespace raycore {
	namespace prim {

		LightDistribution::LightDistribution()
		{
		}


		LightDistribution::~LightDistribution()
		{
			for (Light *light : lights)
				delete light;
		}
		void LightDistribution::addLight(Light * light)
		{
			lights.push_back(light);
		}
		colorHDR LightDistribution::sampleOneLight(const HitInfo & info, const Scene & scene) const
		{
			unsigned int iLight;
			float contribFactor = 1.f;
			if (lights.size() == 0)
				return colorHDR(0.f);
			else if (lights.size() == 1)
			{
				iLight = 0;
			}
			else
			{
				std::vector<float> contributions;
				float globalContribution = 0.f;
				for (Light *light : lights)
				{
					contributions.push_back(light->contribution(info));
					globalContribution += contributions.back();
				}
				float z = rand::rnd();
				float cdf = 0.f;
				for (size_t iContrib = 0; iContrib < contributions.size(); iContrib++)
				{
					cdf += contributions[iContrib] / globalContribution;
					if (z < cdf)
					{
						iLight = iContrib;
						contribFactor = contributions[iContrib];
						break;
					}
				}
			}
			Light *sampledLight = lights[iLight];
			float pdf;
			vec3 ls;
			if (!sampledLight->sample(info, scene, &pdf, &ls))
				return colorHDR(0.f);
			colorHDR brdf = sampledLight->shade();

			return brdf * dot(normalize(ls), info.normal) / (pdf * contribFactor);
		}
	}
}