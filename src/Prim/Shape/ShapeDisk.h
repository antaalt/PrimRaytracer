#pragma once

#include "../Core/Shape.h"

namespace prim {

class ShapeDisk : public Shape {
public:
	ShapeDisk(const point3f &center, float radius, const norm3f &up);
	vec3f sample(const point3f &from) const override;
	float pdf() const override;
	point3f position() const override;
private:
	norm3f m_up;
	point3f m_center;
	float m_radius;
};

}