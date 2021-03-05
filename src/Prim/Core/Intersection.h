#pragma once

#include "Ray.h"

#include <climits>

namespace prim {

class Material;
class Hitable;

struct Intersection {
	Intersection();

	// Is intersection valid ?
	bool valid() const { return material != nullptr; }
	// Report an intersection. Return true if closest to previous report.
	void report(const point3f &hitPoint, const norm3f &normal, const uv2f &texcoord, const Hitable* hitable, const Material *material);
	
	point3f point;
	norm3f normal;
	uv2f texcoord;
	const Hitable* hitable;
	const Material* material;
};

}