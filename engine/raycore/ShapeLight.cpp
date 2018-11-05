#include "ShapeLight.h"



namespace raycore {
	namespace prim {
		ShapeLight::ShapeLight(shape::Shape * shape, colorHDR albedo, float intensity) : Light(albedo, intensity), shape(shape)
		{
		}
		ShapeLight::~ShapeLight()
		{
			delete shape;
		}
		bool ShapeLight::sample(const prim::HitInfo & info, const prim::Scene & scene, float * pdf, vec3 * sample) const
		{
			vec3 s = shape->sample(info.point);
			if (scene.isOccluded(tracer::Ray(info.point, normalize(s), tracer::GEOMETRY_RAY, EPSILON, length(*sample))))
				return false;
			*pdf = shape->pdf();
			*sample = s;
			return true;
		}
		float ShapeLight::contribution(const prim::HitInfo & info) const
		{
			float d = distance(info.point, shape->position());
			return intensity / (d * d);
		}

		namespace shape {

			Sphere::Sphere(const point3 & center, float radius) : center(center), radius(radius)
			{
			}
			vec3 Sphere::sample(const point3 & from) const
			{
				vec3 sample = sample::unitSphere(rand::rnd(), rand::rnd());
				return center - from + sample * radius;
			}
			float Sphere::pdf() const
			{
				return 4.f * M_PIf * radius * radius;;
			}
			point3 Sphere::position() const
			{
				return center;
			}
			Disk::Disk(const point3 & center, float radius, const norm3 &up) : center(center), radius(radius), up(up)
			{
			}
			vec3 Disk::sample(const point3 & from) const
			{
				vec3 sample = sample::unitDisk(rand::rnd(), rand::rnd());
				transform::Onb onb(up);
				return center + onb(sample * radius);
			}
			float Disk::pdf() const
			{
				return M_PIf * radius * radius;
			}
			point3 Disk::position() const
			{
				return center;
			}
		}
	}
}


