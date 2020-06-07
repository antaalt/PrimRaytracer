#include "Hitable.h"

namespace prim {

Hitable::Hitable(const mat4f & transform, Material * material) :
	m_localToWorld(transform),
	m_worldToLocal(m_localToWorld.inverse()),
	m_material(material)
{
}

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