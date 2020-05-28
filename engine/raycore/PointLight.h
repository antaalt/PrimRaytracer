#pragma once
#include "Light.h"

namespace raycore {
namespace prim {

class PointLight : public Light
{
public:
	PointLight(const point3f &center, color4f albedo, float intensity);
	virtual bool sample(const prim::HitInfo &info, const prim::Scene &scene, float *pdf, vec3f *sample) const;
	virtual float contribution(const prim::HitInfo &info) const;
private:
	point3f center;
};

}
}
