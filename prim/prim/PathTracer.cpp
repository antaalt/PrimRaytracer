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

color4f sampleLights(const ComputedIntersection &info, const Scene& scene)
{
	color4f radiance(0.f);
	// TODO sample one light function that return lightID
	for (Light *light : scene.lights)
	{
		Ray shadowRay(light->sample(info.point));
		Culling culling;
		Intersection intersection(culling, true);
		if (scene.intersect(shadowRay, intersection))
			return color4f(0.f);
		float pdf = light->pdf(shadowRay);
		return light->emission() * geo::max(0.f, vec3f::dot(shadowRay.direction, vec3f(info.normal))) / pdf;
	}
	return color4f(0.f);
}

color4f PathTracer::render(const Ray & ray, const Scene & scene) const
{
	uint32_t bounces = 0;
	color4f output(0.f);
	color4f reflectance(1.f);
	Ray rayBounce(ray);
	do
	{
		Culling culling;
		Intersection intersection(culling, false);
		if (!scene.intersect(rayBounce, intersection))
		{
			// Miss
			color4f skyColor(0.9f);
			output += reflectance * skyColor;
			return output;
		}
		ComputedIntersection info = intersection.compute(rayBounce);

		// if material is emissive, return emission
		color4f emission(0.f);

		// Scatter ray
		float pdf;
		vec3f wo;
		BSDFType type;
		const color4f brdf = info.material->sample(info, &wo, &pdf, &type);
		reflectance *= brdf * vec3f::dot(wo, vec3f(info.normal)) / pdf;

		// Sample light (depending on type)
		color4f lightRadiance(0.f);		
		//if (!(type & BSDF_SPECULAR))
		{
			lightRadiance = sampleLights(info, scene);
		}

		// Russian roulette
		if (bounces > (m_maxDepth / 2))
		{
			float probability = geo::max(reflectance.r, geo::max(reflectance.g, reflectance.b));
			if (probability < Rand::sample<float>())
				break;
			reflectance = reflectance / probability;
		}

		output += lightRadiance * reflectance;
		rayBounce.direction = wo;
		rayBounce.origin = info.point;
	} while (++bounces < m_maxDepth);
	return output;
}

}