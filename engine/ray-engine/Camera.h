#pragma once
#include "Config.h"

#include "Ray.h"

namespace app {
	namespace tracer {

		class Camera
		{
		public:
			using Ptr = Camera*;
			Camera(unsigned int width, unsigned int height);
			~Camera();

			virtual Ray generateRay(unsigned int x, unsigned int y) const = 0;

			virtual bool computeTransform() = 0;

			void lookAt(const Point3 &eye, const Point3 &target, const Vector3 &up = Vector3(0.f, 1.f, 0.f));
			void rotate(float angle, const Vector3 &axis);
			void translate(const Vector3 &translation);

		protected:
			Matrix4 m_transform;
			unsigned int m_width, m_height;
			float m_fov;
			bool m_changed;
		};
	}
}
