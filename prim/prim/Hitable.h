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
	Hitable(Material *material);

	// Build the hitable if needed
	virtual void build() {}
	// Intersect the hitable and report any intersection
	virtual bool intersect(const Ray &ray, Intersection *intersection) const = 0;
	// Include the hitable in the bounding box
	virtual void include(BoundingBox &boundingBox) const = 0;

	// Set the material of the hitable
	void setMaterial(Material *material);
	// Get the material of the hitable
	Material *getMaterial() const;
protected:
	Material *m_material; // Material of the hitable
};
}