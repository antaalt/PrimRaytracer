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
			norm3 u, v, w;
			point3 eye;
			float aperture, focalDist;
		};
	}
}

