#include "WhittedTracer.h"
#include "Material.h"
#include "Light.h"

namespace prim {

WhittedTracer::WhittedTracer(uint32_t maxDepth) :
	m_maxDepth(maxDepth)
{
}

geometry::color4f WhittedTracer::render(const Ray & ray, const Scene & scene) const
{
	static uint32_t depth = 0;
	depth++;
	uint32_t bounces = 0;
	color4f output(0);
	Culling culling;
	Ray rayBounce(ray);
	rayBounce.culling = &culling;
	do
	{
		Intersection intersection(false);
		if (!scene.intersect(rayBounce, intersection))
		{
			depth--;
			return geometry::color4f(1.f);
		}
		ComputedIntersection info = intersection.compute(rayBounce);
		float pdf;
		vec3f wo;
		BSDFType type;
		color4f color = info.material->sample(info, &wo, &pdf, &type);
		output += color * vec3f::dot(wo, vec3f(0,1,0));


		switch (info.material->getType())
		{
		default:
		case prim::BSDFType::BSDF_GLOSSY:
		case prim::BSDFType::BSDF_DIFFUSE:
			depth--;
			return output;
		case prim::BSDFType::BSDF_SPECULAR:
		case prim::BSDFType::BSDF_REFLECTION:
		{
			vec3f refractedDirection;
			vec3f reflectedDirection = geo::reflect(rayBounce.direction, info.normal);
			float eta = 1.1f;
			bool inside = (vec3f::dot(rayBounce.direction, vec3f(info.normal)) > 0.f);
			norm3f n;
			if (inside)
			{
				eta = 1.f / eta;
				n = info.normal;
			}
			else
			{
				n = -info.normal;
			}
			if (geo::refract(refractedDirection, rayBounce.direction, n, eta))
			{
				depth--;
				return render(Ray(info.point, reflectedDirection), scene);
			}
			Schlick schlick(eta, 1.f / eta);
			float R = schlick.evaluate(rayBounce.direction, n);
			//float R = physics::fresnel_schlick(vec3f::dot(rayBounce.direction, vec3f(n)));
			Ray refractedRay(info.point, refractedDirection);
			Ray reflectedRay(info.point, reflectedDirection);
			depth--;
			return R * render(reflectedRay, scene) + (1.f - R) * render(refractedRay, scene);
		}
		case prim::BSDFType::BSDF_TRANSMISSION:
		{
			Ray nextRay(info.point, geo::reflect(rayBounce.direction, info.normal));
			depth--;
			return output * render(nextRay, scene);
		}
		}
	} while (++bounces < m_maxDepth && depth < 5);

	depth--;
	return output;
}

}

