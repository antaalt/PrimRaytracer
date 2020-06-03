#pragma once
#include "Ray.h"
#include "BoundingBox.h"
#include "Config.h"
#include "Intersection.h"
#include "Transform.h"

#include <climits>
#include <functional>

namespace prim {

class Material;

class Hitable {
public:
	Hitable(const mat4f &transform, Material *material) : m_transform(transform), m_material(material) {}

	// Build the hitable if needed
	virtual void build() {}
	// Intersect the hitable and report any intersection
	virtual bool intersect(const Ray &ray, Intersection &intersection) const = 0;
	// Include the hitable in the bounding box
	virtual void include(BoundingBox &boundingBox) = 0;

	// Set the transform of the hitable
	// TODO move this to external node ?
	void setTransform(const Transform &transform);
	// Get the transform of the hitable
	const Transform &getTransform() const;
	// Set the material of the hitable
	void setMaterial(Material *material);
	// Get the material of the hitable
	Material *getMaterial() const;
protected:
	friend struct Intersection;
	// Compute the intersection for the primitive at indice.
	virtual void compute(const point3f &hitPoint, const vec2f &barycentric, Intersection::Indice indice, norm3f *normal, uv2f *texCoord, color4f *color) const = 0;
protected:
	Transform m_transform; // Transform local to world of the hitable
	Material *m_material; // Material of the hitable
};
}