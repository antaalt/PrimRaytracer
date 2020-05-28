#pragma once
#include "Ray.h"
#include "Hitable.h"
#include "Scene.h"
#include "PixelBuffer.h"
#include "Config.h"

//#define MAX_DEPTH 10
#define BACKGROUND_COLOR geometry::color4f(0.1f)

namespace raycore {

namespace tracer {

class Tracer
{
public:
	virtual geometry::color4f castRay(const Ray &ray, const prim::Scene &scene, unsigned int depth = RAY_DEFAULT_DEPTH) const = 0;
	virtual void postProcess() {};
	virtual void reset() {};

private:
	virtual bool trace(const Ray &ray, const prim::Scene &scene, prim::HitInfo &info) const = 0;
	virtual geometry::color4f miss(const Ray &ray) const = 0;

};

}

}


