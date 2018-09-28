#pragma once
#include "Ray.h"
#include "Camera.h"

namespace raycore {

	namespace tracer {
		class PinholeCamera : public Camera
		{
		public:
			PinholeCamera();
			~PinholeCamera();

			virtual Ray generateUnitRay(RayIndex x, RayIndex y) const;

			virtual bool computeTransform();

		private:
			vec3 u, v, w, eye;
		};
	}
}

