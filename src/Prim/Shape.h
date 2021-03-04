#pragma once

#include "Config.h"

namespace prim {

class Shape {
public:
	// return non normalized sample
	virtual vec3f sample(const point3f &from) const = 0;
	// return pdf of the shape
	virtual float pdf() const = 0;
	// return point3 as position
	virtual point3f position() const = 0;
};

}
