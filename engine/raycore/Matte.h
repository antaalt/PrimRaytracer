#pragma once
#include "Material.h"
	
namespace raycore {
	namespace prim {

		class Matte : public Material {
		public:
			Matte(Texture<colorHDR> *color);
			virtual colorHDR sample(const tracer::Ray &in, const prim::HitInfo &info, vec3 &wo, float &pdf, BSDFType &type) const;
		};
	}
}


