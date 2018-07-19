#pragma once
#include "Ray.h"


namespace Application::RayTracer {
		
	class Material;

	struct Intersection {
		bool hit;
		Point3 point;
		Normal normal;
		Material *material;
		Intersection() : material(nullptr), hit(false) {};
	};

	class Hitable
	{
	public:
		virtual bool intersect(const Ray &ray, Intersection &intersection) const = 0;
	};
}