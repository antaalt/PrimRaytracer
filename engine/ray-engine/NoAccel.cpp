#include "NoAccel.h"



namespace Application {
	namespace RayTracer {
		NoAccel::NoAccel()
		{
		}
		NoAccel::~NoAccel()
		{
		}
		bool NoAccel::build(const Scene & scene)
		{
			m_scene = &scene;
			return true;
		}
		bool NoAccel::intersect(const Ray & ray, Intersection & intersection) const
		{
			float minDistance = std::numeric_limits<float>::max();
			for (size_t iPrimitive = 0; iPrimitive < m_scene->primitives.size(); iPrimitive++)
			{
				const Primitive &prim = m_scene->primitives[iPrimitive];
				Intersection testIntersection;
				if (prim.intersect(ray, testIntersection))
				{
					if (intersection.distance < minDistance && intersection.distance > ray.tmin && intersection.distance < ray.tmax)
					{
						minDistance = intersection.distance;
						intersection = testIntersection;
					}
				}
			}
			if (minDistance < std::numeric_limits<float>::max())
				return true;
			else
				return false;
		}
	}
}

