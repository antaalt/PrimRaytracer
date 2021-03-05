#include "Intersection.h"

#include "Hitable.h"

namespace prim {

Intersection::Intersection() :
	point(0.f),
	normal(0.f),
	texcoord(0.f),
	hitable(nullptr),
	material(nullptr)
{
}

void Intersection::report(const point3f & hitPoint, const norm3f & normal, const uv2f & texcoord, const Hitable* hitable, const Material * material)
{
	this->point = hitPoint;
	this->normal = normal;
	this->texcoord = texcoord;
	this->hitable = hitable;
	this->material = material;
}

}