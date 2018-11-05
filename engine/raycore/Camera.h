#pragma once
#include "Config.h"

#include "Ray.h"
#include "Random.h"


namespace raycore {
	namespace tracer {

		/*using Aperture = raycore::IndexType<float, struct aperture>;

		struct Film {
			unsigned int width, height;
		};

		struct Lens {
			Aperture aperture;
			
		};*/

		enum class RaySampler {
			LINEAR,
			RANDOM,
			RANDOM_TENT_FILTER
		};

		// Generate RayIndex for the Camera include between interval ]-1;1[
		struct RayIndex {
			RayIndex(unsigned int index, unsigned int dim, RaySampler s = RaySampler::LINEAR)
			{
				switch (s)
				{
				case RaySampler::LINEAR:
					value = (static_cast<float>(index) + 0.5f) / static_cast<float>(dim);
					break;
				case RaySampler::RANDOM:
					value = (static_cast<float>(index) + rand::rnd()) / static_cast<float>(dim);
					break;
				default:
					break;
				}
				value = value * 2.f - 1.f;
			}
			float get() { return value; }

		private:
			float value;
		};

		class Camera
		{
		public:
			Camera();
			~Camera();

			virtual Ray generateRay(RayIndex x, RayIndex y) const = 0;

			virtual bool computeTransform() = 0;

			void lookAt(const point3 &eye, const point3 &target, const vec3 &up = vec3(0.f, 1.f, 0.f));
			void rotate(float angle, const vec3 &axis);
			void translate(const vec3 &translation);

			void setFov(float fov);

		protected:
			mat4 m_transform;
			float m_fov;
			bool m_changed;
		};
	}
}
