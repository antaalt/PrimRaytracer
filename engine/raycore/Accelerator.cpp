#include "Accelerator.h"

#include <map>
#include "Material.h"
#include "Triangle.h"
#include "Sphere.h"
#include "PointLight.h"
#include "ConstantTexture.h"

namespace raycore {

	namespace prim {

		Accelerator::Accelerator(const std::vector<Hitable*>& hitables)
		{
			this->hitables = hitables;
			for (Hitable* hitable : hitables)
 				this->bbox.include(hitable->computeBoundingBox());
		}
	}
}
