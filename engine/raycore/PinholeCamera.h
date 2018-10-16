#pragma once
#include "Ray.h"
#include "Camera.h"

namespace raycore {

	namespace tracer {
		class PinholeCamera : public Camera
		{
		public:
			PinholeCamera(float aperture, float focalDist);
			~PinholeCamera();

			virtual Ray generateRay(RayIndex x, RayIndex y) const;

			virtual bool computeTransform();

		private:
			vec3 u, v, w, eye;
			float aperture, focalDist;
		};
	}
}

