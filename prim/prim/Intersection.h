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
	// Ray info
	vec3f direction; // incident ray direction
	point3f point; // hit point of ray
	// Object info
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
	// Get the culling
	Culling &getCulling() const { return m_culling; }
	// Get the distance
	float getDistance() const { return m_distance; }
	// Get the distance
	vec2f getBarycentric() const { return m_barycentric; }
	// Get the distance
	Indice getIndice() const { return m_indice; }
private:
	bool m_terminateOnFirstHit; // Terminate on first hit
	Culling &m_culling;
	float m_distance;
	vec2f m_barycentric;
	const Hitable *m_hitable;
	Indice m_indice; // Indice of primitive hit inside hitable
};

}