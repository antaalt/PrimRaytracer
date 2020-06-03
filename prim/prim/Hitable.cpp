#include "Hitable.h"

namespace prim {

void Hitable::setTransform(const Transform & transform)
{
	m_transform = transform;
}

const Transform & Hitable::getTransform() const
{
	return m_transform;
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