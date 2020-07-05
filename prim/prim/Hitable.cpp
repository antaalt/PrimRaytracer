#include "Hitable.h"

namespace prim {

Hitable::Hitable(Material * material) :
	m_material(material)
{
}

void Hitable::setMaterial(Material * material)
{
	m_material = material;
}

Material * Hitable::getMaterial() const
{
	return m_material;
}

}