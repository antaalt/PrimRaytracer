#include "Hitable.h"

namespace prim {

void Hitable::setTransform(const mat4f & transform)
{
	m_transform = transform;
}

const mat4f & Hitable::getTransform() const
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