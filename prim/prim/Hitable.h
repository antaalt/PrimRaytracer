#pragma once
#include "Ray.h"
#include "BoundingBox.h"
#include "Config.h"

#include <climits>
#include <functional>

namespace prim {

class Material;
class Hitable;
struct Intersection;

struct ComputedIntersection {
	vec3f direction;
	point3f point;
	norm3f normal;
	uv2f texcoord;
	color4f color;
	Material *material;
};

using AnyHit = std::function<void(const Intersection &)>;

struct Intersection {
	Intersection();

	float distance;
	vec2f barycentric;
	const Hitable *hitable;
	const void *data; // handle for passing custom data depending on hitable

	bool valid() const { return distance < (std::numeric_limits<float>::max)(); }
	bool report(float distance, vec2f barycentric, const Hitable *hitable, const void *data = nullptr);
};

struct Culling {
	virtual bool operator()(float cosTheta) {
		return geometry::abs(cosTheta) < std::numeric_limits<float>::epsilon();
	}
};

struct FrontCulling : Culling {
	bool operator()(float cosTheta) override {
		return cosTheta > std::numeric_limits<float>::epsilon();
	}
};

struct BackCulling : Culling {
	bool operator()(float cosTheta) override {
		return cosTheta < std::numeric_limits<float>::epsilon();
	}
};

class Hitable {
public:
	Hitable(const mat4f &transform, Material *material) : m_transform(transform), m_material(material) {}

	virtual void build() {}

	virtual bool intersect(const Ray &ray, Intersection *intersection) const = 0;

	virtual ComputedIntersection compute(const Ray &ray, const Intersection &intersection) const = 0;

	virtual void include(BoundingBox &boundingBox) = 0;
public:
	void setTransform(const mat4f &transform);
	const mat4f &getTransform() const;

protected:
	virtual float area() const { return 0.f; }
protected:
	mat4f m_transform;
	Material *m_material;
};
}