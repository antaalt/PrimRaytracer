#pragma once
#include "Ray.h"

#include <climits>


namespace Application::RayTracer {
		
	class Material;

	struct Intersection {
		bool hit;
		Point3 point;
		Normal normal;
		Texcoord texCoord;
		float distance;
		Material *material;
		Intersection() : material(nullptr), hit(false), distance(std::numeric_limits<float>::max()) {};
	};

	class Hitable
	{
	public:
		virtual bool intersect(const Ray &ray, Intersection &intersection) const = 0;
	};
}