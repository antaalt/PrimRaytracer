#pragma once

#include "Hitable.h"

namespace prim {

class TransformNode : public Hitable {
public:
	TransformNode(const mat4f &transform, Hitable *hitable);
	~TransformNode();

	void build() override;
	bool intersect(const Ray &ray, Intersection *intersection) const override;
	void include(BoundingBox &boundingBox) const override;

	void setTransform(const Transform &transform);
	const Transform &getTransform() const;
	const Hitable *getHitable() const { return m_hitable; }
private:
	Transform m_localToWorld;
	Transform m_worldToLocal;
	Hitable *m_hitable;
};

}
