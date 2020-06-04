#include "Hitable.h"

namespace prim {

void Hitable::setTransform(const Transform & transform)
{
	m_localToWorld = transform;
	m_worldToLocal = m_localToWorld.inverse();
}

const Transform & Hitable::getTransform() const
{
	return m_localToWorld;
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