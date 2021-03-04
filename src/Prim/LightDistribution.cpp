#include "LightDistribution.h"
#include "Scene.h"

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
color4f LightDistribution::sampleOneLight(const Intersection & intersection, const Scene & scene) const
{
	/*unsigned int iLight;
	float contribFactor = 1.f;
	if (lights.size() == 0)
		return color4f(0.f);
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
		float z = Rand::sample<float>();
		float cdf = 0.f;
		for (size_t iContrib = 0; iContrib < contributions.size(); iContrib++)
		{
			cdf += contributions[iContrib] / globalContribution;
			if (z < cdf)
			{
				iLight = static_cast<unsigned int>(iContrib);
				contribFactor = contributions[iContrib];
				break;
			}
		}
	}
	Light *sampledLight = lights[iLight];
	float pdf;
	vec3f ls;
	if (!sampledLight->sample(info, scene, &pdf, &ls))
		return color4f(0.f);
	color4f brdf = sampledLight->shade();

	return brdf * vec3f::dot(vec3f::normalize(ls), geometry::vec3f(info.normal)) / (pdf * contribFactor);*/
	return color4f();
}
}