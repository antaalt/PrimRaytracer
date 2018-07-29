#pragma once
#include "Accelerator.h"

#include "Material.h"
#include "BoundingBox.h"
#include "Hitable.h"

namespace app {
	namespace tracer {

		class NoAccel : public Accelerator
		{
		public:
			NoAccel();
			~NoAccel();
			virtual bool build(const Scene &scene);

			virtual bool intersect(const Ray &ray, prim::HitInfo &intersection) const;

		private:
			std::vector<prim::Hitable::Ptr> m_hitable;
			std::vector<prim::Material::Ptr> m_materials;
			size_t m_hitableCount;
			std::vector<Texture> m_textures;
			prim::BoundingBox bbox;
		};
	}
}


