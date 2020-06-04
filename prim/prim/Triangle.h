#pragma once

#include "Hitable.h"
#include "Config.h"

#include <array>

namespace prim {

norm3f interpolate(const norm3f &nA, const norm3f &nB, const norm3f &nC, vec2f barycentric);
uv2f interpolate(const uv2f &tA, const uv2f &tB, const uv2f &tC, vec2f barycentric);
color4f interpolate(const color4f &cA, const color4f &cB, const color4f &cC, vec2f barycentric);

class Triangle : public Hitable
{
public:
	struct Vertex {
		point3f position;
		norm3f normal;
		uv2f texcoord;
		color4f color;
	};
public:
	bool intersect(const Ray &ray, Intersection &intersection) const override;
	
	void compute(const point3f &hitPoint, const vec2f &barycentric, Intersection::Indice indice, norm3f *normal, uv2f *texCoord, color4f *color) const override;
protected:
	float area() const;
private:
	union {
		Vertex data[3];
		struct {
			Vertex A, B, C;
		};
	};
};

}
