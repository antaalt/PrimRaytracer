#pragma once
#include "Material.h"

namespace raycore {
	namespace prim {

		class Metal : public Material {
		public:
			Metal(Texture<colorHDR> *color, Texture<float> *roughness);

			virtual colorHDR sample(const tracer::Ray &in, const prim::HitInfo &info, vec3 &wo, float &pdf, BSDFType &type) const;
		private:
			Texture<float> *roughness;
		};
	}
}
