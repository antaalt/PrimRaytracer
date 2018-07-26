#pragma once
#include "Accelerator.h"

namespace Application {
	namespace RayTracer {
		class NoAccel : public Accelerator
		{
		public:
			NoAccel();
			~NoAccel();
			virtual bool build(const Scene &scene);

			virtual bool intersect(const Ray &ray, Intersection &intersection) const;

		private:
			const Scene *m_scene;
		};
	}
}


