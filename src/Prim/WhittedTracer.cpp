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
		Intersection intersection;
		if (!scene.intersect(rayBounce, intersection))
		{
			depth--;
			return geometry::color4f(1.f);
		}
		float pdf;
		vec3f wo;
		BSDFType type;
		color4f color = intersection.material->sample(intersection, rayBounce.direction, &wo, &pdf, &type);
		output += color * vec3f::dot(wo, vec3f(0,1,0));


		switch (intersection.material->getType())
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
			vec3f reflectedDirection = reflect(rayBounce.direction, intersection.normal);
			float eta = 1.1f;
			bool inside = (vec3f::dot(rayBounce.direction, vec3f(intersection.normal)) > 0.f);
			norm3f n;
			if (inside)
			{
				eta = 1.f / eta;
				n = intersection.normal;
			}
			else
			{
				n = -intersection.normal;
			}
			if (refract(refractedDirection, rayBounce.direction, n, eta))
			{
				depth--;
				return render(Ray(intersection.point, reflectedDirection), scene);
			}
			Schlick schlick(eta, 1.f / eta);
			float R = schlick.evaluate(rayBounce.direction, n);
			//float R = physics::fresnel_schlick(vec3f::dot(rayBounce.direction, vec3f(n)));
			Ray refractedRay(intersection.point, refractedDirection);
			Ray reflectedRay(intersection.point, reflectedDirection);
			depth--;
			return R * render(reflectedRay, scene) + (1.f - R) * render(refractedRay, scene);
		}
		case prim::BSDFType::BSDF_TRANSMISSION:
		{
			Ray nextRay(intersection.point, reflect(rayBounce.direction, intersection.normal));
			depth--;
			return output * render(nextRay, scene);
		}
		}
	} while (++bounces < m_maxDepth && depth < 5);

	depth--;
	return output;
}

}

