#include "WhittedTracer.h"
#include "Material.h"
#include "Light.h"

namespace prim {
namespace tracer {

WhittedTracer::WhittedTracer()
{
}

WhittedTracer::~WhittedTracer()
{
}
geometry::color4f WhittedTracer::castRay(const Ray & ray, const Scene &scene, unsigned int depth) const
{
	if (depth == 0)
		return miss(ray);
	ComputedIntersection info;
	if (!scene.intersect(ray, &info))
		return miss(ray); 
			
	float pdf = 0;
	return geometry::color4f();
	/*colorHDR reflectance = info.material->color(info.texcoord.x, info.texcoord.y) * info.color;
	colorHDR radiance(0.f);
	for (size_t iLight = 0; iLight < accelerator->getLightsCount(); iLight++)
	{
		const Light *l = accelerator->getLight(iLight);
		LightInfo linfo;
		if (l->sample(info, accelerator, linfo))
		{
			radiance = linfo.color * std::abs(vec3::dot(linfo.sample, info.normal));
		}
	}
	reflectance = reflectance + reflectance * radiance;
	switch (info.material->type())
	{
	case prim::MaterialType::DIFFUSE:
		return reflectance;
	case prim::MaterialType::DIELECTRIC:
	{
		vec3 refractedDirection;
		vec3 reflectedDirection = prim::reflect(ray.direction, info.normal);
		float eta = 1.1f;
		bool inside = (vec3::dot(ray.direction, info.normal) > 0.f);
		norm3 n;
		if (inside)
		{
			eta = 1.f / eta;
			n = info.normal;
		}
		else
		{
			n = -info.normal;
		}
		if (prim::refract(refractedDirection, ray.direction, n, eta))
			return castRay(Ray(info.point, reflectedDirection), accelerator, depth - 1);

		float R = physics::fresnel_schlick(vec3::dot(ray.direction, n));
		Ray refractedRay(info.point, refractedDirection);
		Ray reflectedRay(info.point, reflectedDirection);
		return R * castRay(reflectedRay, accelerator, depth - 1) + (1.f - R) * castRay(refractedRay, accelerator, depth - 1);
	}
	case prim::MaterialType::SPECULAR:
	{
		Ray nextRay(info.point, prim::reflect(ray.direction, info.normal));
		return Pixel(reflectance) + castRay(nextRay, accelerator, depth - 1);
	}
	case prim::MaterialType::METAL:
	default:
		return miss(ray);
	}*/
}

geometry::color4f WhittedTracer::miss(const Ray & ray) const
{
	//return Pixel((ray.direction.x + 1.f) / 2.f, (ray.direction.y + 1.f) / 2.f, (ray.direction.z + 1.f) / 2.f, 1.f);
	return BACKGROUND_COLOR;
	//return Pixel(vec3::dot(ray.direction, vec3(0.f, 1.f, 0.f)));
}

}
}

