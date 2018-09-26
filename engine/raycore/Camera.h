#pragma once
#include "Config.h"

#include "Ray.h"
#include "Utils/Type.h"

namespace raycore {
	namespace tracer {

		using Aperture = raycore::IndexType<float, struct aperture>;

		struct Film {
			unsigned int width, height;
		};

		struct Lens {
			Aperture aperture;
			
		};


		class Camera
		{
		public:
			using Ptr = Camera*;
			Camera(unsigned int width, unsigned int height);
			~Camera();

			virtual Ray generateRay(unsigned int x, unsigned int y) const = 0;

			virtual bool computeTransform() = 0;

			void lookAt(const point3 &eye, const point3 &target, const vec3 &up = vec3(0.f, 1.f, 0.f));
			void rotate(float angle, const vec3 &axis);
			void translate(const vec3 &translation);

		protected:
			mat4 m_transform;
			unsigned int m_width, m_height;
			float m_fov;
			bool m_changed;
		};
	}
}
