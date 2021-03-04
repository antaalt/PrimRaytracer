#pragma once

#include <vector>

#include "../../Core/Hitable.h"
#include "Triangle.h"

namespace prim {

class Mesh : public Hitable {
public:
	Mesh(Material *material);

	void addTriangle(const Triangle &tri) { m_triangles.push_back(tri); }
	void addTriangle(Triangle &&tri) { m_triangles.push_back(std::move(tri)); }
	
	void build() override;

	bool intersect(const Ray &ray, Intersection *intersection) const override;

	void include(BoundingBox &boundingBox) const override;
protected:
	BoundingBox m_bbox;
	std::vector<Triangle> m_triangles;
};

}
