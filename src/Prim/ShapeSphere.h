#pragma once

#include "Shape.h"

namespace prim {

class ShapeSphere : public Shape {
public:
	ShapeSphere(const point3f &center, float radius);
	vec3f sample(const point3f &from) const override;
	float pdf() const override;
	point3f position() const override;
private:
	float m_radius;
	point3f m_center;
};

}