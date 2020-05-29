#include "PathTracer.h"
#include "Hitable.h"
#include "Material.h"
#include "Random.h"
#include "Light.h"

namespace raycore {

namespace tracer {

PathTracer::PathTracer()
{
}

geometry::color4f sampleLights(const ComputedIntersection &info, const Scene& scene)
{
	geometry::color4f radiance(0.f);
	//const prim::LightDistribution *lightDistribution = scene.getLightDistribution();
	/*for (size_t iLight = 0; iLight < lightDistribution.; iLight++)
	{
		const Light *l = accelerator->getLight(iLight);
		LightInfo linfo;
		if (l->sample(info, accelerator, linfo))
		{
			radiance = radiance + linfo.color * std::abs(dot(linfo.sample, info.normal));
		}
	}*/
	//return lightDistribution->sampleOneLight(info, scene);
	return radiance;
}

geometry::color4f PathTracer::castRay(const Ray & ray, const Scene& scene, unsigned int depth) const
{
	if (depth == 0)
		return geometry::color4f(0.f);
	ComputedIntersection info;
	if (!scene.intersect(ray, &info))
		return miss(ray);

	// if material is emissive, return emission
	geometry::color4f emission(0.f);

	// Scatter ray
	float pdf;
	geometry::vec3f wo;
	BSDFType type;
	geometry::color4f brdf = info.material->sample(ray, info, wo, pdf, type) * info.color;
	geometry::color4f reflectance = brdf * geometry::vec3f::dot(wo, geometry::vec3f(info.normal)) / pdf;

	// Sample light (depending on type)
	geometry::color4f lightRadiance(0.f);
	if (!(type & BSDF_SPECULAR))
	{
		lightRadiance = reflectance * sampleLights(info, scene);
	}

	// Russian roulette
	if (depth < (RAY_DEFAULT_DEPTH / 2))
	{
		float probability = geometry::max(reflectance.r, geometry::max(reflectance.g, reflectance.b));
		if (probability < Rand::sample<float>())
			return geometry::color4f(0.f);
		reflectance = reflectance / probability;
	}
	//return colorHDR(wo.x, wo.y, wo.z, 1.f);
	//return colorHDR(info.point.x, info.point.y, info.point.z, 1.f);

	geometry::color4f ret = lightRadiance + reflectance * castRay(Ray(info.point, wo), scene, depth - 1);
	return ret;
}

geometry::color4f PathTracer::miss(const Ray & ray) const
{
	return geometry::color4f(0.9f);
}
}
}