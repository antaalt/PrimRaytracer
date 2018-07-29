#pragma once
#include "Ray.h"
#include "Camera.h"

namespace app {

	namespace tracer {
		class PinholeCamera : public Camera
		{
		public:
			PinholeCamera(unsigned int width, unsigned int height);
			~PinholeCamera();

			virtual Ray generateRay(unsigned int x, unsigned int y) const;

			virtual bool computeTransform();

		private:
			Vector3 u, v, w, eye;
		};
	}
}

