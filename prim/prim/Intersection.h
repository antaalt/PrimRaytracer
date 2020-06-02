#pragma once

#include "Ray.h"

#include <climits>

namespace prim {

class Material;
class Hitable;
struct Intersection;

struct Culling {
	virtual bool operator()(float cosTheta) { return geometry::abs(cosTheta) < std::numeric_limits<float>::epsilon(); }
};

struct FrontCulling : Culling {
	bool operator()(float cosTheta) override { return cosTheta > std::numeric_limits<float>::epsilon(); }
};

struct BackCulling : Culling {
	bool operator()(float cosTheta) override { return cosTheta < std::numeric_limits<float>::epsilon(); }
};

struct ComputedIntersection {
	vec3f direction;
	point3f point;
	norm3f normal;
	uv2f texcoord;
	color4f color;
	Material *material;
};

struct Intersection {
	using Indice = uint32_t; // Only support a maximum of max(uint32) hitable primitive

	Intersection(Culling &culling, bool terminateOnFirstHit);

	// Cull object
	bool cull(float cosTheta) const { return m_culling(cosTheta); }
	// Is intersection valid ?
	bool valid() const { return m_distance < (std::numeric_limits<float>::max)(); }
	// Is intersection finishing on first hit ?
	bool terminateOnFirstHit() const { return m_terminateOnFirstHit; }
	// Report an intersection. Return true if closest to previous report.
	bool report(float distance, vec2f barycentric, const Hitable *hitable, Indice indice = 0);
	// Compute the intersection.
	ComputedIntersection compute(const Ray &ray) const;
private:
	bool m_terminateOnFirstHit; // Terminate on first hit
	Culling &m_culling;
	float m_distance;
	vec2f m_barycentric;
	const Hitable *m_hitable;
	Indice m_indice; // Indice of primitive hit inside hitable
};

}