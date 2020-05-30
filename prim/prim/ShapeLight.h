#pragma once
#include "Config.h"
#include "Random.h"
#include "Light.h"

namespace prim {

namespace shape
{
	class Shape {
	public:
		// return non normalized sample
		virtual vec3f sample(const point3f &from) const = 0;
		// return pdf of the shape
		virtual float pdf() const = 0;
		// return point3 as position
		virtual point3f position() const = 0;
	};

	class Sphere : public Shape {
	public:
		Sphere(const point3f &center, float radius);
		virtual vec3f sample(const point3f &from) const;
		virtual float pdf() const;
		virtual point3f position() const;
	private:
		float radius;
		point3f center;
	};
	class Disk : public Shape {
	public:
		Disk(const point3f &center, float radius, const norm3f &up);
		virtual vec3f sample(const point3f &from) const;
		virtual float pdf() const;
		virtual point3f position() const;
	private:
		norm3f up;
		point3f center;
		float radius;
	};
}

class ShapeLight : public Light
{
public:
	ShapeLight(shape::Shape *shape, color4f albedo, float intensity);
	~ShapeLight();
	virtual bool sample(const ComputedIntersection &info, const Scene &scene, float *pdf, vec3f *sample) const;
	float contribution(const ComputedIntersection &info) const;
private:
	shape::Shape *shape;
};

}
