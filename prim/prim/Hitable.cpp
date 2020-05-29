#include "Hitable.h"

namespace prim {

Intersection::Intersection() :
	hitable(nullptr),
	distance(std::numeric_limits<float>::max()),
	barycentric(0.f)
{
}

bool Intersection::report(float distance, vec2f barycentric, const Hitable * hitable, const void *data)
{
	if (this->distance <= distance)
		return false;
	this->distance = distance;
	this->barycentric = barycentric;
	this->hitable = hitable;
	this->data = data;
	return true;
}

void Hitable::setTransform(const mat4f & transform)
{
	m_transform = transform;
}

const mat4f & Hitable::getTransform() const
{
	return m_transform;
}

}