#include "Camera.h"
#include "Transform.h"

namespace prim {

RaySampler::Type LinearRaySampler::operator()(vec2u position, vec2u size)
{
	const vec2f texcoord = (vec2f(position) + vec2f(0.5f)) / vec2f(size);
	const vec2f screenPos = texcoord * 2.f - vec2f(1.f);
	return Type(screenPos.x, -screenPos.y);
}

RaySampler::Type RandomRaySampler::operator()(vec2u position, vec2u size)
{
	uint32_t sample = 0; // TODO pass sample

	const uint32_t nbSamples = 16; // This value must be a power of two
	const uint32_t rootNbSamples = geometry::sqrt(nbSamples);
	const uint32_t strate = sample % nbSamples;
	const uint32_t xStrate = strate % rootNbSamples;
	const uint32_t yStrate = strate / rootNbSamples;
	float r1 = Rand::sample<float>();
	float r2 = Rand::sample<float>();
	// Box filtering
	// Value between [0.0, 1.0]
	vec2f subpixelJitter = vec2f(
		(float(xStrate) + r1) / float(rootNbSamples),
		(float(yStrate) + r2) / float(rootNbSamples)
	);

	const vec2f texcoord = (vec2f(position) + subpixelJitter) / vec2f(size); 
	const vec2f screenPos = texcoord * 2.f - vec2f(1.f);
	return Type(screenPos.x, -screenPos.y);
}

}