#include "Scene.h"
#include "Triangle.h"
#include "Sphere.h"

namespace prim {

void Scene::build()
{
	for (TransformNode *node : this->nodes)
	{
		node->build();
		node->include(bbox);
	}
}
bool Scene::intersect(const Ray &ray, Intersection &intersection) const
{
	if (!bbox.intersect(ray))
		return false;
	bool hit = false;
	for (TransformNode *node : this->nodes) {
		if (node->intersect(ray, &intersection)) {
			hit = true;
		}
	}
	return hit;
}

TransformNode::TransformNode(const mat4f & transform, Hitable * hitable) :
	Hitable(nullptr), // No material for this node.
	m_localToWorld(transform),
	m_worldToLocal(m_localToWorld.inverse()),
	m_hitable(hitable)
{
}

TransformNode::~TransformNode()
{
	delete m_hitable;
}

void TransformNode::build()
{
	m_hitable->build();
}

bool TransformNode::intersect(const Ray & ray, Intersection * intersection) const
{
	Ray localRay = m_worldToLocal(ray);
	if (m_hitable->intersect(localRay, intersection))
	{
		intersection->point = m_localToWorld(intersection->point);
		intersection->normal = norm3f::normalize(m_localToWorld(intersection->normal));
		ray.tmax = point3f::distance(ray.origin, intersection->point);
		return true;
	}
	return false;
}

void TransformNode::include(BoundingBox & boundingBox) const
{
	BoundingBox bbox;
	m_hitable->include(bbox);
	boundingBox.include(m_localToWorld(bbox));
}

void TransformNode::setTransform(const Transform & transform)
{
	m_localToWorld = transform; 
	m_worldToLocal = transform.inverse();
}

const Transform & TransformNode::getTransform() const
{
	return m_localToWorld;
}

}