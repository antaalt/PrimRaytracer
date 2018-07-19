#pragma once
#include "Config.h"
#include "Hitable.h"

namespace Application::RayTracer {

	class BoundingBox : public Hitable
	{
	public:
		BoundingBox();
		BoundingBox(const Point3 &min, const Point3 &max);
		~BoundingBox();

		virtual bool intersect(const Ray &ray, Intersection &intersection) const;;

		float extent() const;
		Point3 center() const;

		void include(const Point3 &vec);
		void include(const BoundingBox &bbox);

		void reset();

		Point3 min, max;
	};

}
