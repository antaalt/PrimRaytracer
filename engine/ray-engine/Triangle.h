#pragma once
#include "Hitable.h"

namespace Application::RayTracer {
	class Triangle : public Hitable
	{
	public:
		Triangle();
		~Triangle();

		virtual bool intersect(const Ray &ray, Intersection &intersection) const;

		Point3 A, B, C;
		Normal normal;
	};
}

