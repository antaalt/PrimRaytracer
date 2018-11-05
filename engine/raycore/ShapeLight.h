#pragma once
#include "Config.h"
#include "Sampling.h"
#include "Random.h"
#include "Onb.h"
#include "Light.h"
namespace raycore {
	namespace prim {
		namespace shape
		{
			class Shape {
			public:
				// return non normalized sample
				virtual vec3 sample(const point3 &from) const = 0;
				// return pdf of the shape
				virtual float pdf() const = 0;
				// return point3 as position
				virtual point3 position() const = 0;
			};

			class Sphere : public Shape {
			public:
				Sphere(const point3 &center, float radius);
				virtual vec3 sample(const point3 &from) const;
				virtual float pdf() const;
				virtual point3 position() const;
			private:
				float radius;
				point3 center;
			};
			class Disk : public Shape {
			public:
				Disk(const point3 &center, float radius, const norm3 &up);
				virtual vec3 sample(const point3 &from) const;
				virtual float pdf() const;
				virtual point3 position() const;
			private:
				norm3 up;
				point3 center;
				float radius;
			};
		}

		class ShapeLight : public Light
		{
		public:
			ShapeLight(shape::Shape *shape, colorHDR albedo, float intensity);
			~ShapeLight();
			virtual bool sample(const prim::HitInfo &info, const prim::Scene &scene, float *pdf, vec3 *sample) const;
			float contribution(const prim::HitInfo &info) const;
		private:
			shape::Shape *shape;
		};
	}
}
