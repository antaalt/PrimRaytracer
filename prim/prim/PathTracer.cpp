#include "PathTracer.h"
#include "Hitable.h"
#include "Material.h"
#include "Random.h"
#include "Light.h"

namespace prim {

PathTracer::PathTracer(uint32_t maxDepth) :
	m_maxDepth(maxDepth)
{
}

geometry::color4f sampleLights(const ComputedIntersection &info, const Scene& scene)
{
	geometry::color4f radiance(0.f);
	/*const prim::LightDistribution *lightDistribution = scene.getLightDistribution();
	for (size_t iLight = 0; iLight < lightDistribution.; iLight++)
	{
		const Light *l = accelerator->getLight(iLight);
		LightInfo linfo;
		if (l->sample(info, accelerator, linfo))
		{
			radiance = radiance + linfo.color * std::abs(dot(linfo.sample, info.normal));
		}
	}
	return lightDistribution->sampleOneLight(info, scene);*/
	return radiance;
}

geometry::color4f prim::PathTracer::render(const Ray & ray, const Scene & scene) const
{
	uint32_t bounces = 0;
	geometry::color4f output(0.f);
	geometry::color4f reflectance(1.f);
	Ray rayBounce(ray);
	do
	{
		ComputedIntersection info;
		if (!scene.intersect(rayBounce, &info))
			return reflectance * geometry::color4f(0.9f);

		// if material is emissive, return emission
		geometry::color4f emission(0.f);

		// Scatter ray
		float pdf;
		geometry::vec3f wo;
		BSDFType type;
		const geometry::color4f brdf = info.material->sample(info, &wo, &pdf, &type);
		reflectance *= brdf * geometry::vec3f::dot(wo, geometry::vec3f(info.normal)) / pdf;

		// Sample light (depending on type)
		geometry::color4f lightRadiance(0.f);
		if (!(type & BSDF_SPECULAR))
		{
			lightRadiance = reflectance * sampleLights(info, scene);
		}

		// Russian roulette
		if (bounces > (m_maxDepth / 2))
		{
			float probability = geometry::max(reflectance.r, geometry::max(reflectance.g, reflectance.b));
			if (probability < Rand::sample<float>())
				return geometry::color4f(0.f);
			reflectance = reflectance / probability;
		}

		output += lightRadiance * reflectance;
		rayBounce.direction = wo;
		rayBounce.origin = info.point;
	} while (++bounces < m_maxDepth);
	return output;
}

}