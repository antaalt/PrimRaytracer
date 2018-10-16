#pragma once
#include "Material.h"

namespace raycore {
	namespace prim {
		class Glass : public Material {
		public:
			Glass(Texture<colorHDR> *texture, float eta);

			virtual colorHDR sample(const tracer::Ray &in, const prim::HitInfo &info, vec3 &wo, float &pdf, BSDFType &type) const;
		private:
			float ior;
		};
	}
}
