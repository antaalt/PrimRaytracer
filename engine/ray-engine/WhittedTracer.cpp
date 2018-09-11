#include "WhittedTracer.h"
#include "Material.h"
#include "Math.h"

namespace app {
	namespace tracer {

		WhittedTracer::WhittedTracer()
		{
		}

		WhittedTracer::~WhittedTracer()
		{
		}
		Pixel WhittedTracer::castRay(const Ray & ray, const Accelerator::Ptr accelerator, unsigned int depth) const
		{
			if (depth > 1)// > MAX_DEPTH)
				return miss(ray);
			prim::HitInfo info;
			if (!trace(ray, accelerator, info))
				return miss(ray);

			float pdf = 0;
			color4 color = info.material->color(info.texcoord.x, info.texcoord.y) * info.color * vec3::dot(info.normal, -ray.direction);
			switch (info.material->type())
			{
			case prim::MaterialType::DIFFUSE:
				return color;
			case prim::MaterialType::DIELECTRIC:
			{
				vec3 reflectedDirection;
				vec3 refractedDirection;
				const bool inside = (vec3::dot(info.normal, ray.direction) > 0.f);
				float eta = 1.5f; // TODO get from material
				float cos_theta;
				if (inside) // going out of the medium
				{
					if (prim::refract(refractedDirection, ray.direction, -info.normal, eta))
						return castRay(Ray(info.point, prim::reflect(ray.direction, -info.normal)), accelerator, ++depth); // TIR
					cos_theta = vec3::dot(ray.direction, info.normal);
					reflectedDirection = prim::reflect(ray.direction, -info.normal);
				}
				else // going in the medium
				{
					prim::refract(refractedDirection, ray.direction, info.normal, 1.f / eta);
					cos_theta = vec3::dot(-ray.direction, info.normal);
					reflectedDirection = prim::reflect(ray.direction, info.normal);
				}
				float R = physics::fresnel(cos_theta, eta);
				Ray refractedRay(info.point, refractedDirection);
				Ray reflectedRay(info.point, reflectedDirection);
				//return Pixel(info.normal.x, info.normal.y, info.normal.z, 1.f);
				//return Pixel(inside);
				return/* R * castRay(reflectedRay, accelerator, ++depth) + (1 - R) **/ castRay(refractedRay, accelerator, ++depth);
			}
			case prim::MaterialType::SPECULAR:
			{
				Ray nextRay(info.point, prim::reflect(ray.direction, info.normal));
				return Pixel(color) + castRay(nextRay, accelerator, ++depth);
			}
			case prim::MaterialType::METAL:
			default:
				return miss(ray);
			}
		}
		bool WhittedTracer::trace(const Ray & ray, const Accelerator::Ptr accelerator, prim::HitInfo & info) const
		{
			return accelerator->intersect(ray, info);
		}
		Pixel WhittedTracer::miss(const Ray & ray) const
		{
			//return Pixel((ray.direction.x + 1.f) / 2.f, (ray.direction.y + 1.f) / 2.f, (ray.direction.z + 1.f) / 2.f, 1.f);
			return BACKGROUND_COLOR;
			//return Pixel(vec3::dot(ray.direction, vec3(0.f, 1.f, 0.f)));
		}
	}
}

