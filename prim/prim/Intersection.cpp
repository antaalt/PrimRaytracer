#include "Intersection.h"

#include "Hitable.h"

namespace prim {

Intersection::Intersection(bool terminateOnFirstHit) :
	m_terminateOnFirstHit(terminateOnFirstHit),
	m_distance(std::numeric_limits<float>::max()),
	m_barycentric(0.f),
	m_hitable(nullptr),
	m_indice(0)
{
}

bool Intersection::report(float distance, vec2f barycentric, const Hitable * hitable, Indice indice)
{
	if (m_distance <= distance)
		return false;
	m_distance = distance;
	m_barycentric = barycentric;
	m_hitable = hitable;
	m_indice = indice;
	return true;
}

ComputedIntersection Intersection::compute(const Ray & ray) const
{ 
	ComputedIntersection computedIntersection;
	computedIntersection.point = ray(m_distance);
	computedIntersection.direction = ray.direction;
	computedIntersection.material = m_hitable->getMaterial();
	m_hitable->compute(computedIntersection.point, m_barycentric, m_indice, &computedIntersection.normal, &computedIntersection.texcoord);
	return computedIntersection;
}

}