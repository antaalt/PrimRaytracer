#pragma once
#include "Config.h"

#include "Ray.h"
#include "Random.h"


namespace raycore {
namespace tracer {

/*using Aperture = raycore::IndexType<float, struct aperture>;

struct Film {
	unsigned int width, height;
};

struct Lens {
	Aperture aperture;
			
};*/

enum class RaySampler {
	LINEAR,
	RANDOM,
	RANDOM_TENT_FILTER
};

// Generate RayIndex for the Camera include between interval ]-1;1[
struct RayIndex {
	RayIndex(unsigned int index, unsigned int dim, RaySampler s = RaySampler::LINEAR)
	{
		switch (s)
		{
		case RaySampler::LINEAR:
			value = (static_cast<float>(index) + 0.5f) / static_cast<float>(dim);
			break;
		case RaySampler::RANDOM:
			value = (static_cast<float>(index) + rand::rnd()) / static_cast<float>(dim);
			break;
		default:
			break;
		}
		value = value * 2.f - 1.f;
	}
	float get() { return value; }

private:
	float value;
};

class Camera
{
public:
	Camera();
	~Camera();

	virtual Ray generateRay(RayIndex x, RayIndex y) const = 0;

	virtual bool computeTransform() = 0;

	void lookAt(const point3f &eye, const point3f &target, const vec3f &up = settings::up);
	void rotate(const vec3f &axis, geometry::degreef angle);
	void translate(const vec3f &translation);

	void setFov(float fov);

protected:
	mat4f m_transform;
	float m_fov;
	bool m_changed;
};
}
}
