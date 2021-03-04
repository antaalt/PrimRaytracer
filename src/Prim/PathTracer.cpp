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

color4f sampleLights(const Intersection &info, const Scene& scene)
{
	color4f radiance(0.f);
	// TODO sample one light function that return lightID
	for (Light *light : scene.lights)
	{
		Culling culling;
		Ray shadowRay(light->sample(info.point));
		shadowRay.culling = &culling;
		Intersection intersection;
		if (scene.intersect(shadowRay, intersection))
			return color4f(0.f);
		float pdf = light->pdf(shadowRay);
		return light->emission() * max(0.f, vec3f::dot(shadowRay.direction, vec3f(info.normal))) / pdf;
	}
	return color4f(0.f);
}

color4f PathTracer::render(const Ray & ray, const Scene & scene) const
{
	uint32_t bounces = 0;
	color4f output(0.f);
	color4f reflectance(1.f);
	Culling culling;
	Ray rayBounce(ray);
	rayBounce.culling = &culling;
	do
	{
		Intersection intersection;
		if (!scene.intersect(rayBounce, intersection))
		{
			// Miss
			color4f skyColor(0.9f);
			output += reflectance * skyColor;
			return output;
		}
		//return color4f(intersection.point.x, intersection.point.y, intersection.point.z, 1.f);
		// if material is emissive, return emission
		color4f emission(0.f);

		// Scatter ray
		float pdf;
		vec3f wo;
		BSDFType type;
		const color4f brdf = intersection.material->sample(intersection, rayBounce.direction, &wo, &pdf, &type);
		reflectance *= brdf * vec3f::dot(wo, vec3f(intersection.normal)) / pdf;

		// Sample light (depending on type)
		color4f lightRadiance(0.f);		
		//if (!(type & BSDF_SPECULAR))
		{
			lightRadiance = sampleLights(intersection, scene);
		}

		// Russian roulette
		if (bounces > (m_maxDepth / 2))
		{
			float probability = max(reflectance.r, max(reflectance.g, reflectance.b));
			if (probability < Rand::sample<float>())
				break;
			reflectance = reflectance / probability;
		}

		output += lightRadiance * reflectance;
		rayBounce.direction = wo;
		rayBounce.origin = intersection.point;
		rayBounce.tmax = std::numeric_limits<float>::max();
	} while (++bounces < m_maxDepth);
	return output;
}

}